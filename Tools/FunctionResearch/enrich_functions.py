#!/usr/bin/env python3
"""Merge a Ghidra function export with symbols, confirmed offsets, and MWDT notes.

The source CSV is read-only. Every generated file is written beneath --output.
"""
from __future__ import annotations

import argparse
import csv
import json
import re
from collections import Counter, defaultdict
from pathlib import Path

FIELDS = ["address", "ghidra_name", "friendly_name", "category", "signature",
          "calling_convention", "size", "status", "confidence", "danger_level",
          "description", "evidence", "source", "test_notes", "hit_count", "last_test"]


def address(value: str) -> str:
    text = str(value or "").strip().lower().replace("0x", "")
    if not re.fullmatch(r"[0-9a-f]+", text):
        return ""
    return f"0x{int(text, 16):08X}"


def load_symbols(paths: list[Path]) -> dict[str, list[str]]:
    found: dict[str, list[str]] = defaultdict(list)
    for path in paths:
        if not path.exists():
            continue
        with path.open(encoding="utf-8-sig", errors="replace") as handle:
            for line in handle:
                parts = line.strip().split()
                if len(parts) >= 2:
                    addr = address(parts[1])
                    if addr and parts[0] not in found[addr]:
                        found[addr].append(parts[0])
    return found


def load_offsets(path: Path | None) -> dict[str, str]:
    found = {}
    if not path or not path.exists():
        return found
    pattern = re.compile(r"constexpr\s+std::uintptr_t\s+(\w+)\s*=\s*(0x[0-9A-Fa-f]+)")
    for name, raw in pattern.findall(path.read_text(encoding="utf-8", errors="replace")):
        found[address(raw)] = name
    return found


def load_research(path: Path | None) -> dict[str, dict[str, str]]:
    found = {}
    if not path or not path.exists():
        return found
    with path.open(newline="", encoding="utf-8-sig", errors="replace") as handle:
        for row in csv.DictReader(handle):
            addr = address(row.get("address", ""))
            if addr:
                found[addr] = {str(k).strip(): str(v or "").strip() for k, v in row.items() if k}
    return found


def write_csv(path: Path, rows: list[dict[str, str]]) -> None:
    with path.open("w", newline="", encoding="utf-8-sig") as handle:
        writer = csv.DictWriter(handle, fieldnames=FIELDS, extrasaction="ignore")
        writer.writeheader(); writer.writerows(rows)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("functions", type=Path)
    parser.add_argument("--symbols", type=Path, nargs="*", default=[])
    parser.add_argument("--offsets", type=Path)
    parser.add_argument("--research", type=Path)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()

    symbols = load_symbols(args.symbols)
    offsets = load_offsets(args.offsets)
    research = load_research(args.research)
    rows, by_address = [], defaultdict(list)
    with args.functions.open(newline="", encoding="utf-8-sig", errors="replace") as handle:
        for source in csv.DictReader(handle):
            addr = address(source.get("Location", ""))
            ghidra = source.get("Name", "").strip()
            note = research.get(addr, {})
            symbolic = [name for name in symbols.get(addr, []) if not name.upper().startswith("FUN_")]
            known = offsets.get(addr, "")
            friendly = note.get("friendly_name") or note.get("name") or known or (symbolic[0] if symbolic else "")
            status = note.get("status") or ("VERIFIED" if known else "UNKNOWN")
            confidence = note.get("confidence") or ("CONFIRMED" if known else "LOW")
            evidence = note.get("evidence", "")
            if known:
                evidence = (evidence + "; " if evidence else "") + "ClientOffsets_8606.hpp / commit 641d799"
            row = {field: "" for field in FIELDS}
            row.update({"address": addr, "ghidra_name": ghidra, "friendly_name": friendly,
                        "category": note.get("category") or ("Lua" if known.startswith("Lua") or known.startswith("FrameScript") else "Unknown"),
                        "signature": note.get("signature") or source.get("Function Signature", "").strip(),
                        "calling_convention": note.get("calling_convention", ""),
                        "size": note.get("size") or source.get("Function Size", "").strip(),
                        "status": status, "confidence": confidence,
                        "danger_level": note.get("danger_level") or ("SAFE_READ" if known == "GlobalLuaStatePointer" else "INSTRUMENT_ONLY"),
                        "description": note.get("description", ""), "evidence": evidence,
                        "source": note.get("source") or "Ghidra Functions.csv",
                        "test_notes": note.get("test_notes", ""), "hit_count": note.get("hit_count", "0"),
                        "last_test": note.get("last_test", "")})
            rows.append(row); by_address[addr].append(row)

    args.output.mkdir(parents=True, exist_ok=True)
    write_csv(args.output / "functions_enriched.csv", rows)
    write_csv(args.output / "functions_unknown.csv", [r for r in rows if r["status"] == "UNKNOWN"])
    write_csv(args.output / "functions_verified.csv", [r for r in rows if r["status"] == "VERIFIED"])
    write_csv(args.output / "functions_by_category.csv", sorted(rows, key=lambda r: (r["category"], r["address"])))

    named = sum(not r["ghidra_name"].upper().startswith("FUN_") for r in rows)
    stats = {"total_native_functions": len(rows), "named": named, "fun_auto_named": len(rows)-named,
             "verified": sum(r["status"] == "VERIFIED" for r in rows),
             "candidates": sum(r["status"] == "CANDIDATE" for r in rows),
             "duplicate_address_rows": sum(len(group)-1 for group in by_address.values() if len(group)>1),
             "categories": dict(sorted(Counter(r["category"] for r in rows).items()))}
    (args.output / "statistics.json").write_text(json.dumps(stats, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(stats, indent=2))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
