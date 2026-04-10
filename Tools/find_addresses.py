#!/usr/bin/env python3
"""
TBC Address Finding Toolkit
============================
Searches Docs/symbols/func.sym and Docs/symbols/wow_tbc.sym for candidate
addresses matching functions that are still TODO_TBC in the TBC-Extensions
codebase (WoW 2.4.3 build 8606).

Usage:
    python Tools/find_addresses.py [--verbose] [--output Docs/ADDRESS_FINDINGS.md]

Requirements:
    Python 3.8+  |  No external dependencies
"""

import argparse
import csv
import json
import os
import re
import sys
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, Iterable, List, Optional, Set, Tuple

# ---------------------------------------------------------------------------
# Paths (relative to repo root)
# ---------------------------------------------------------------------------

REPO_ROOT = Path(__file__).resolve().parent.parent
FUNC_SYM   = REPO_ROOT / "Docs" / "symbols" / "func.sym"
WOW_TBC_SYM = REPO_ROOT / "Docs" / "symbols" / "wow_tbc.sym"
FUNCTIONS_CSV = REPO_ROOT / "Docs" / "symbols" / "Functions.csv"
ADDRESS_MAP_JSON = REPO_ROOT / "Docs" / "symbols" / "address_map.json"
OUTPUT_MD  = REPO_ROOT / "Docs" / "ADDRESS_FINDINGS.md"
ALL_OUTPUT_MD = REPO_ROOT / "Docs" / "ADDRESS_ALL_POSSIBILITIES.md"
SOURCE_ROOT = REPO_ROOT / "TBCExtensions" / "src"

# ---------------------------------------------------------------------------
# Data structures
# ---------------------------------------------------------------------------

@dataclass
class Candidate:
    address: str          # e.g. "0x004BA8A0"
    name: str             # symbol name in the sym file
    confidence: int       # 0-100
    method: str           # how we found it
    notes: str = ""


@dataclass
class FunctionTarget:
    name: str                          # canonical name used in the code
    wotlk_address: Optional[str]       # WotLK 3.3.5 address for reference
    priority: str                      # CRITICAL / HIGH / MEDIUM / LOW
    description: str
    search_terms: List[str] = field(default_factory=list)
    candidates: List[Candidate] = field(default_factory=list)
    tbc_address: str = "TODO_TBC"


@dataclass
class SourcePlaceholder:
    target_name: str
    file_path: Path
    line_number: int
    comment: str
    context_function: str = ""
    wotlk_address: Optional[str] = None
    candidates: List[Candidate] = field(default_factory=list)


# ---------------------------------------------------------------------------
# Priority-ranked function catalogue
# ---------------------------------------------------------------------------

FUNCTION_TARGETS: List[FunctionTarget] = [
    # ── CRITICAL ────────────────────────────────────────────────────────────
    FunctionTarget(
        name="FrameScript__LoadFunctions",
        wotlk_address="0x5120E0",
        priority="CRITICAL",
        description="Registers all default Lua API functions at startup. "
                    "Build-breaking if missing.",
        search_terms=["FrameScript__LoadFunctions", "FrameScript_LoadFunctions",
                      "Script_Load", "LoadFunctions"],
    ),
    FunctionTarget(
        name="FrameScript__RegisterFunction",
        wotlk_address="0x817F90",
        priority="CRITICAL",
        description="Maps string→function pointer for Lua API. "
                    "Every custom Lua function needs this.",
        search_terms=["FrameScript__RegisterFunction", "RegisterFunction",
                      "FrameScript_Register"],
    ),
    FunctionTarget(
        name="FrameScript__GetState",
        wotlk_address="0x00884BAC",
        priority="CRITICAL",
        description="Returns the global lua_State* pointer.",
        search_terms=["FrameScript__GetState", "GetState", "lua_State"],
    ),
    FunctionTarget(
        name="CVar__Set",
        wotlk_address="0x766940",
        priority="CRITICAL",
        description="Sets a CVar value. Used extensively by extension code.",
        search_terms=["CVar__Set", "CVar_Set"],
    ),
    FunctionTarget(
        name="SErr__PrepareAppFatal",
        wotlk_address="0x772A80",
        priority="CRITICAL",
        description="Fatal error handler. Build-breaking if wrong address.",
        search_terms=["SErr__PrepareAppFatal", "SErr_PrepareAppFatal",
                      "PrepareAppFatal"],
    ),
    # ── HIGH ────────────────────────────────────────────────────────────────
    FunctionTarget(
        name="Script_GetSpellInfo",
        wotlk_address=None,
        priority="HIGH",
        description="Lua API: GetSpellInfo(spellID) → name, rank, icon, …",
        search_terms=["Script_GetSpellInfo", "GetSpellInfo"],
    ),
    FunctionTarget(
        name="Script_UnitGUID",
        wotlk_address=None,
        priority="HIGH",
        description="Lua API: UnitGUID(unit) → GUID string.",
        search_terms=["Script_UnitGUID", "UnitGUID"],
    ),
    FunctionTarget(
        name="Script_GetItemInfo",
        wotlk_address=None,
        priority="HIGH",
        description="Lua API: GetItemInfo(itemID|link) → name, link, …",
        search_terms=["Script_GetItemInfo", "GetItemInfo"],
    ),
    FunctionTarget(
        name="Script_UnitHealth",
        wotlk_address=None,
        priority="HIGH",
        description="Lua API: UnitHealth(unit) → current HP.",
        search_terms=["Script_UnitHealth", "UnitHealth"],
    ),
    FunctionTarget(
        name="Script_GetCameraPosition",
        wotlk_address=None,
        priority="HIGH",
        description="Lua API: GetCameraPosition() → x, y, z.",
        search_terms=["Script_GetCameraPosition", "GetCameraPosition",
                      "CameraPos"],
    ),
    # ── Lua C-API (HIGH) ────────────────────────────────────────────────────
    FunctionTarget(
        name="lua_error",
        wotlk_address="0x84F280",
        priority="HIGH",
        description="Lua C-API: raises a Lua error (FrameScript::DisplayError).",
        search_terms=["lua_error", "luaG_runerror"],
    ),
    FunctionTarget(
        name="lua_toboolean",
        wotlk_address="0x84E0B0",
        priority="HIGH",
        description="Lua C-API: converts stack value to boolean.",
        search_terms=["lua_toboolean"],
    ),
    FunctionTarget(
        name="lua_tonumber",
        wotlk_address="0x84E030",
        priority="HIGH",
        description="Lua C-API: converts stack value to double.",
        search_terms=["lua_tonumber"],
    ),
    FunctionTarget(
        name="FrameScript__GetParam",
        wotlk_address="0x815500",
        priority="HIGH",
        description="WoW wrapper: pops a parameter with optional boolean coercion.",
        search_terms=["FrameScript__GetParam", "FrameScript_GetParam",
                      "GetParam"],
    ),
    FunctionTarget(
        name="lua_tolstring",
        wotlk_address="0x84E0E0",
        priority="HIGH",
        description="Lua C-API: converts stack value to C string.",
        search_terms=["lua_tolstring", "lua_tostring"],
    ),
    FunctionTarget(
        name="FrameScript__GetText",
        wotlk_address="0x819D40",
        priority="HIGH",
        description="WoW localization lookup by string key.",
        search_terms=["FrameScript__GetText", "FrameScript_GetText",
                      "GetText"],
    ),
    FunctionTarget(
        name="lua_gettop",
        wotlk_address="0x84DBD0",
        priority="HIGH",
        description="Lua C-API: returns the number of items on the stack.",
        search_terms=["lua_gettop"],
    ),
    FunctionTarget(
        name="lua_isnumber",
        wotlk_address="0x84DF20",
        priority="HIGH",
        description="Lua C-API: tests whether a stack value is a number.",
        search_terms=["lua_isnumber"],
    ),
    FunctionTarget(
        name="lua_isstring",
        wotlk_address="0x84DF60",
        priority="HIGH",
        description="Lua C-API: tests whether a stack value is a string.",
        search_terms=["lua_isstring"],
    ),
    FunctionTarget(
        name="lua_pushboolean",
        wotlk_address="0x84E4D0",
        priority="HIGH",
        description="Lua C-API: pushes a boolean onto the stack.",
        search_terms=["lua_pushboolean"],
    ),
    FunctionTarget(
        name="lua_pushnil",
        wotlk_address="0x84E280",
        priority="HIGH",
        description="Lua C-API: pushes nil onto the stack.",
        search_terms=["lua_pushnil"],
    ),
    FunctionTarget(
        name="lua_pushnumber",
        wotlk_address="0x84E2A0",
        priority="HIGH",
        description="Lua C-API: pushes a double onto the stack.",
        search_terms=["lua_pushnumber"],
    ),
    FunctionTarget(
        name="lua_pushstring",
        wotlk_address="0x84E350",
        priority="HIGH",
        description="Lua C-API: pushes a C string onto the stack.",
        search_terms=["lua_pushstring"],
    ),
    FunctionTarget(
        name="lua_settop",
        wotlk_address="0x84DBF0",
        priority="HIGH",
        description="Lua C-API: sets the stack top (removes excess values).",
        search_terms=["lua_settop"],
    ),
    FunctionTarget(
        name="FrameScript__SignalEvent",
        wotlk_address="0x81B530",
        priority="HIGH",
        description="Fires a WoW Lua event (e.g. UNIT_HEALTH).",
        search_terms=["FrameScript__SignalEvent", "FrameScript_SignalEvent",
                      "SignalEvent"],
    ),
    FunctionTarget(
        name="lua_getfield",
        wotlk_address="0x84E620",
        priority="HIGH",
        description="Lua C-API: gets a field from a table on the stack.",
        search_terms=["lua_getfield"],
    ),
    FunctionTarget(
        name="lua_setfield",
        wotlk_address="0x84E680",
        priority="HIGH",
        description="Lua C-API: sets a field in a table on the stack.",
        search_terms=["lua_setfield"],
    ),
    FunctionTarget(
        name="lua_pushcclosure",
        wotlk_address="0x84E460",
        priority="HIGH",
        description="Lua C-API: pushes a C closure (function) onto the stack.",
        search_terms=["lua_pushcclosure"],
    ),
    FunctionTarget(
        name="lua_type",
        wotlk_address="0x84DEC0",
        priority="HIGH",
        description="Lua C-API: returns the type of a stack value.",
        search_terms=["lua_type"],
    ),
    # ── MEDIUM ──────────────────────────────────────────────────────────────
    FunctionTarget(
        name="SStr__Printf",
        wotlk_address="0x76F070",
        priority="MEDIUM",
        description="Storm string printf.",
        search_terms=["SStr__Printf", "SStr_Printf"],
    ),
    FunctionTarget(
        name="SStr__Append",
        wotlk_address="0x76EF70",
        priority="MEDIUM",
        description="Storm string append.",
        search_terms=["SStr__Append", "SStr_Append"],
    ),
    FunctionTarget(
        name="SStr__Copy",
        wotlk_address="0x76ED20",
        priority="MEDIUM",
        description="Storm string copy.",
        search_terms=["SStr__Copy", "SStr_Copy"],
    ),
    FunctionTarget(
        name="SFile__CloseFile",
        wotlk_address="0x422910",
        priority="MEDIUM",
        description="Storm file close.",
        search_terms=["SFile__CloseFile", "SFile_CloseFile", "CloseFile"],
    ),
    FunctionTarget(
        name="CFormula__GetVariableValue",
        wotlk_address="0x5782D0",
        priority="MEDIUM",
        description="Returns a formula variable value (used in spell descriptions).",
        search_terms=["CFormula__GetVariableValue", "CFormula_GetVariableValue",
                      "GetVariableValue"],
    ),
    FunctionTarget(
        name="DBClient__GetGameTableValue",
        wotlk_address="0x7F6990",
        priority="MEDIUM",
        description="Returns a value from a game table DBC.",
        search_terms=["DBClient__GetGameTableValue", "GetGameTableValue"],
    ),
    FunctionTarget(
        name="DBClient__GetLocalizedRow",
        wotlk_address="0x4CFD20",
        priority="MEDIUM",
        description="Returns a localised DBC row.",
        search_terms=["DBClient__GetLocalizedRow", "GetLocalizedRow"],
    ),
    FunctionTarget(
        name="CGChat__AddChatMessage",
        wotlk_address="0x509DD0",
        priority="MEDIUM",
        description="Adds a message to the in-game chat frame.",
        search_terms=["CGChat__AddChatMessage", "CGChat_AddChatMessage",
                      "AddChatMessage"],
    ),
    FunctionTarget(
        name="SpellParser__ParseText",
        wotlk_address=None,
        priority="MEDIUM",
        description="Parses a spell description template string.",
        search_terms=["SpellParser__ParseText", "SpellParser_ParseText",
                      "ParseText"],
    ),
    FunctionTarget(
        name="CGUnit__GetShapeshiftFormID",
        wotlk_address=None,
        priority="MEDIUM",
        description="Returns the active shapeshift form ID for a unit.",
        search_terms=["CGUnit__GetShapeshiftFormID", "GetShapeshiftFormID"],
    ),
    FunctionTarget(
        name="CGPlayer__IsDeadOrGhost",
        wotlk_address=None,
        priority="MEDIUM",
        description="Returns true if the local player is dead or a ghost.",
        search_terms=["CGPlayer__IsDeadOrGhost", "IsDeadOrGhost"],
    ),
    FunctionTarget(
        name="CGPetInfo__GetPet",
        wotlk_address=None,
        priority="MEDIUM",
        description="Returns the GUID of the pet at a given index.",
        search_terms=["CGPetInfo__GetPet", "GetPet"],
    ),
    # ── LOW ─────────────────────────────────────────────────────────────────
    FunctionTarget(
        name="DNInfo__AddZoneLight",
        wotlk_address=None,
        priority="LOW",
        description="Adds a zone light entry to the day/night info struct.",
        search_terms=["DNInfo__AddZoneLight", "AddZoneLight"],
    ),
    FunctionTarget(
        name="DNInfo__GetDNInfoPtr",
        wotlk_address=None,
        priority="LOW",
        description="Returns a pointer to the global DNInfo structure.",
        search_terms=["DNInfo__GetDNInfoPtr", "GetDNInfoPtr"],
    ),
    FunctionTarget(
        name="CWorld__LoadMap",
        wotlk_address=None,
        priority="LOW",
        description="Loads a game map by directory/position/mapID.",
        search_terms=["CWorld__LoadMap", "CWorld_LoadMap", "LoadMap"],
    ),
    FunctionTarget(
        name="CWorld__UnloadMap",
        wotlk_address=None,
        priority="LOW",
        description="Unloads the current game map.",
        search_terms=["CWorld__UnloadMap", "CWorld_UnloadMap", "UnloadMap"],
    ),
    FunctionTarget(
        name="CWorld__Pos3Dto2D",
        wotlk_address=None,
        priority="LOW",
        description="Projects a 3-D world position onto the 2-D minimap.",
        search_terms=["CWorld__Pos3Dto2D", "Pos3Dto2D"],
    ),
]

# ---------------------------------------------------------------------------
# Symbol file loader
# ---------------------------------------------------------------------------

def load_sym_file(path: Path) -> Dict[str, str]:
    """Return {name: address_hex} from a .sym file."""
    symbols: Dict[str, str] = {}
    if not path.exists():
        print(f"[WARN] Symbol file not found: {path}", file=sys.stderr)
        return symbols
    with path.open(encoding="utf-8", errors="replace") as fh:
        for line in fh:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            parts = line.split()
            if len(parts) >= 2:
                name, addr = parts[0], parts[1].upper()
                if not addr.startswith("0X"):
                    addr = "0x" + addr
                symbols[name] = addr
    return symbols


def load_functions_csv(path: Path) -> Dict[str, Tuple[str, str]]:
    """Return {name: (address, signature)} from Functions.csv."""
    data: Dict[str, Tuple[str, str]] = {}
    if not path.exists():
        return data
    with path.open(encoding="utf-8", errors="replace", newline="") as fh:
        reader = csv.reader(fh)
        next(reader, None)  # skip header
        for row in reader:
            if len(row) >= 3:
                name = row[0].strip()
                addr = row[1].strip().upper()
                sig  = row[2].strip()
                if not addr.startswith("0X"):
                    addr = "0x" + addr
                data[name] = (addr, sig)
    return data

# ---------------------------------------------------------------------------
# Search helpers
# ---------------------------------------------------------------------------

def _normalise(name: str) -> str:
    """Lower-case and strip double underscores for fuzzy matching."""
    return re.sub(r"[_\-]+", "_", name.lower())


def _tokenise(name: str) -> List[str]:
    cleaned = re.sub(r"[^A-Za-z0-9]+", "_", name)
    parts = [p.lower() for p in cleaned.split("_") if p]
    tokens: List[str] = []

    for part in parts:
        camel = re.findall(r"[A-Z]?[a-z]+|[A-Z]+(?=[A-Z]|$)|\d+", part)
        if camel:
            tokens.extend(piece.lower() for piece in camel if piece)
        else:
            tokens.append(part)

    return [t for t in tokens if t]


def _symbol_variants(name: str) -> Set[str]:
    tokens = _tokenise(name)
    variants = {
        name,
        name.replace("::", "__"),
        name.replace("__", "_"),
        name.replace("::", "_"),
        _normalise(name),
    }

    if tokens:
        variants.add("_".join(tokens))
        variants.add("".join(tokens))

    prefixes = [
        "script", "framescript", "lua", "cvar", "sfile", "sstr", "cgplayer",
        "cgunit", "cgchat", "cgpetinfo", "dbclient", "cformula", "dninfo",
        "cworld",
    ]

    for prefix in prefixes:
        if tokens[:1] == [prefix]:
            remainder = tokens[1:]
            if remainder:
                variants.add("_".join(remainder))
                variants.add("".join(remainder))

    return {variant for variant in variants if variant}


def _score_token_overlap(target_tokens: List[str], symbol_tokens: List[str]) -> int:
    if not target_tokens or not symbol_tokens:
        return 0

    overlap = len(set(target_tokens) & set(symbol_tokens))
    if overlap == 0:
        return 0

    coverage = overlap / len(set(target_tokens))
    score = int(45 + coverage * 35)

    if overlap == len(set(target_tokens)):
        score += 10

    return min(score, 88)


def search_exact(term: str,
                 named: Dict[str, str],
                 all_syms: Dict[str, str]) -> Optional[Candidate]:
    """Exact name match in either symbol set."""
    for sym_dict, label in [(named, "func.sym"), (all_syms, "wow_tbc.sym")]:
        if term in sym_dict:
            return Candidate(
                address=sym_dict[term],
                name=term,
                confidence=95,
                method=f"exact_match:{label}",
            )
    return None


def search_case_insensitive(term: str,
                             named: Dict[str, str],
                             all_syms: Dict[str, str]) -> List[Candidate]:
    """Case-insensitive name match."""
    results: List[Candidate] = []
    term_lower = term.lower()
    for sym_dict, label in [(named, "func.sym"), (all_syms, "wow_tbc.sym")]:
        for name, addr in sym_dict.items():
            if name.lower() == term_lower:
                results.append(Candidate(
                    address=addr,
                    name=name,
                    confidence=90,
                    method=f"case_insensitive:{label}",
                ))
    return results


def search_normalised(term: str,
                      named: Dict[str, str],
                      all_syms: Dict[str, str]) -> List[Candidate]:
    """Normalised exact-ish matches across common separators/prefix styles."""
    results: List[Candidate] = []
    variants = {_normalise(v) for v in _symbol_variants(term)}

    for sym_dict, label in [(named, "func.sym"), (all_syms, "wow_tbc.sym")]:
        for name, addr in sym_dict.items():
            if _normalise(name) in variants:
                results.append(Candidate(
                    address=addr,
                    name=name,
                    confidence=88 if label == "func.sym" else 82,
                    method=f"normalised_match:{label}",
                ))

    return results


def search_substring(term: str,
                     named: Dict[str, str],
                     all_syms: Dict[str, str]) -> List[Candidate]:
    """Substring / pattern match."""
    results: List[Candidate] = []
    term_lower = term.lower()
    for sym_dict, label in [(named, "func.sym"), (all_syms, "wow_tbc.sym")]:
        for name, addr in sym_dict.items():
            if term_lower in name.lower():
                conf = 75 if label == "func.sym" else 50
                results.append(Candidate(
                    address=addr,
                    name=name,
                    confidence=conf,
                    method=f"substring:{label}",
                ))
    return results


def search_token_overlap(term: str,
                         named: Dict[str, str],
                         all_syms: Dict[str, str]) -> List[Candidate]:
    """Token-based fuzzy match, useful for name-style differences."""
    results: List[Candidate] = []
    target_tokens = _tokenise(term)

    for sym_dict, label in [(named, "func.sym"), (all_syms, "wow_tbc.sym")]:
        for name, addr in sym_dict.items():
            symbol_tokens = _tokenise(name)
            score = _score_token_overlap(target_tokens, symbol_tokens)
            if score >= 60:
                if label == "wow_tbc.sym":
                    score = max(55, score - 10)
                results.append(Candidate(
                    address=addr,
                    name=name,
                    confidence=score,
                    method=f"token_overlap:{label}",
                ))

    return results


def search_signature_hints(term: str,
                           functions_csv: Dict[str, Tuple[str, str]]) -> List[Candidate]:
    """Use Ghidra signatures to surface additional plausible matches."""
    results: List[Candidate] = []
    target_tokens = set(_tokenise(term))
    variants = {_normalise(v) for v in _symbol_variants(term)}

    for name, (addr, signature) in functions_csv.items():
        norm_name = _normalise(name)
        sig_tokens = set(_tokenise(signature))

        if norm_name in variants:
            results.append(Candidate(
                address=addr,
                name=name,
                confidence=86,
                method="normalised_match:Functions.csv",
                notes=signature,
            ))
            continue

        overlap = len(target_tokens & sig_tokens)
        if overlap >= 2 or (target_tokens and target_tokens.issubset(sig_tokens)):
            score = min(74, 48 + overlap * 8)
            results.append(Candidate(
                address=addr,
                name=name,
                confidence=score,
                method="signature_hint:Functions.csv",
                notes=signature,
            ))

    return results


def search_proximity(wotlk_addr_str: str,
                     all_syms: Dict[str, str],
                     window: int = 0x500) -> List[Candidate]:
    """
    Find functions in wow_tbc.sym whose address is within `window` bytes of the
    given WotLK address.  Useful as a heuristic when functions tend to live in
    the same region of the binary across versions.
    """
    if not wotlk_addr_str:
        return []
    try:
        wotlk_int = int(wotlk_addr_str, 16)
    except ValueError:
        return []

    results: List[Candidate] = []
    for name, addr_str in all_syms.items():
        try:
            addr_int = int(addr_str, 16)
        except ValueError:
            continue
        dist = abs(addr_int - wotlk_int)
        if dist <= window:
            conf = max(5, 40 - int(dist / (window / 40)))
            results.append(Candidate(
                address=addr_str,
                name=name,
                confidence=conf,
                method=f"proximity_to_wotlk({wotlk_addr_str}):wow_tbc.sym",
                notes=f"distance=0x{dist:X}",
            ))
    results.sort(key=lambda c: int(c.address, 16))
    return results[:5]  # return closest 5


def deduplicate(candidates: List[Candidate]) -> List[Candidate]:
    """Remove duplicate addresses, keeping highest confidence."""
    seen: Dict[str, Candidate] = {}
    for c in candidates:
        key = c.address
        if key not in seen or c.confidence > seen[key].confidence:
            seen[key] = c
    out = list(seen.values())
    out.sort(key=lambda c: c.confidence, reverse=True)
    return out

# ---------------------------------------------------------------------------
# Core search logic
# ---------------------------------------------------------------------------

def find_function(target: FunctionTarget,
                  named: Dict[str, str],
                  all_syms: Dict[str, str],
                  functions_csv: Dict[str, Tuple[str, str]],
                  verbose: bool = False) -> None:
    """Populate target.candidates and target.tbc_address in-place."""
    candidates: List[Candidate] = []

    for term in target.search_terms:
        # 1. Exact match
        c = search_exact(term, named, all_syms)
        if c:
            candidates.append(c)
            if verbose:
                print(f"  [exact] {term} → {c.address} ({c.confidence}%)")

        # 2. Case-insensitive
        for c in search_case_insensitive(term, named, all_syms):
            candidates.append(c)

        # 3. Normalised exact-ish match
        for c in search_normalised(term, named, all_syms):
            candidates.append(c)

        # 4. Substring
        for c in search_substring(term, named, all_syms):
            candidates.append(c)

        # 5. Token overlap
        for c in search_token_overlap(term, named, all_syms):
            candidates.append(c)

        # 6. Signature hints from Functions.csv
        for c in search_signature_hints(term, functions_csv):
            candidates.append(c)

    # 7. Proximity to WotLK address
    if target.wotlk_address:
        for c in search_proximity(target.wotlk_address, all_syms):
            candidates.append(c)

    target.candidates = deduplicate(candidates)

    # If we have a high-confidence hit, record it
    if target.candidates and target.candidates[0].confidence >= 85:
        target.tbc_address = target.candidates[0].address


def _extract_function_target(comment: str, context_function: str) -> Optional[str]:
    patterns = [
        r"Find\s+([A-Za-z_][A-Za-z0-9_:]*)\s+address",
        r"Find\s+([A-Za-z_][A-Za-z0-9_:]*)\s+function\s+address",
        r"Replace\s+0x[0-9A-Fa-f]+\s+with\s+TBC.*?\s+([A-Za-z_][A-Za-z0-9_:]*)\s+address",
    ]

    for pattern in patterns:
        match = re.search(pattern, comment)
        if match:
            return match.group(1).replace("::", "__")

    if context_function and "TODO_TBC" in comment:
        return context_function.replace("::", "__")

    return None


def scan_source_placeholders(source_root: Path) -> List[SourcePlaceholder]:
    placeholders: List[SourcePlaceholder] = []
    function_re = re.compile(r"^\s*[\w:<>\*&\s]+?\s+([A-Za-z_]\w*)::([A-Za-z_]\w*)\s*\(")
    wotlk_re = re.compile(r"WotLK.*?(0x[0-9A-Fa-f]+)")

    for path in source_root.rglob("*.cpp"):
        try:
            lines = path.read_text(encoding="utf-8", errors="replace").splitlines()
        except OSError:
            continue

        current_function = ""

        for idx, line in enumerate(lines, start=1):
            fn_match = function_re.match(line)
            if fn_match:
                current_function = f"{fn_match.group(1)}__{fn_match.group(2)}"

            if "TODO_TBC" not in line:
                continue

            stripped = line.strip()
            target = _extract_function_target(stripped, current_function)
            if not target:
                continue

            wotlk_address = None
            lookahead = lines[idx:min(idx + 3, len(lines))]
            for extra in lookahead:
                match = wotlk_re.search(extra)
                if match:
                    wotlk_address = match.group(1)
                    break

            placeholders.append(SourcePlaceholder(
                target_name=target,
                file_path=path,
                line_number=idx,
                comment=stripped,
                context_function=current_function,
                wotlk_address=wotlk_address,
            ))

    deduped: Dict[Tuple[str, str], SourcePlaceholder] = {}
    for placeholder in placeholders:
        key = (str(placeholder.file_path), placeholder.target_name)
        deduped.setdefault(key, placeholder)

    return sorted(deduped.values(), key=lambda p: (str(p.file_path), p.line_number, p.target_name))


def find_placeholder_candidates(placeholder: SourcePlaceholder,
                                named: Dict[str, str],
                                all_syms: Dict[str, str],
                                functions_csv: Dict[str, Tuple[str, str]]) -> None:
    candidates: List[Candidate] = []

    search_terms = [placeholder.target_name]
    if placeholder.context_function and placeholder.context_function != placeholder.target_name:
        search_terms.append(placeholder.context_function)

    for term in search_terms:
        exact = search_exact(term, named, all_syms)
        if exact:
            candidates.append(exact)
        candidates.extend(search_case_insensitive(term, named, all_syms))
        candidates.extend(search_normalised(term, named, all_syms))
        candidates.extend(search_substring(term, named, all_syms))
        candidates.extend(search_token_overlap(term, named, all_syms))
        candidates.extend(search_signature_hints(term, functions_csv))

    if placeholder.wotlk_address:
        candidates.extend(search_proximity(placeholder.wotlk_address, all_syms))

    placeholder.candidates = deduplicate(candidates)


# ---------------------------------------------------------------------------
# Report generation
# ---------------------------------------------------------------------------

PRIORITY_ORDER = ["CRITICAL", "HIGH", "MEDIUM", "LOW"]


def _priority_emoji(p: str) -> str:
    return {"CRITICAL": "[CRIT]", "HIGH": "[HIGH]", "MEDIUM": "[MED]", "LOW": "[LOW]"}.get(p, "")


def generate_report(targets: List[FunctionTarget], output_path: Path) -> None:
    found    = [t for t in targets if t.tbc_address != "TODO_TBC"]
    missing  = [t for t in targets if t.tbc_address == "TODO_TBC"]
    pct      = int(100 * len(found) / max(len(targets), 1))

    lines: List[str] = [
        "# TBC Address Findings Report",
        "",
        "> Auto-generated by `Tools/find_addresses.py`  ",
        "> Re-run after updating symbol files to refresh candidates.",
        "",
        "## Summary",
        "",
        f"| Metric | Value |",
        f"|--------|-------|",
        f"| Total functions targeted | {len(targets)} |",
        f"| Addresses found (≥ 85% confidence) | {len(found)} |",
        f"| Still missing / unconfirmed | {len(missing)} |",
        f"| Overall progress | **{pct}%** |",
        "",
        "---",
        "",
        "## Functions Found",
        "",
    ]

    if found:
        lines += [
            "| Function | TBC Address | Confidence | Method |",
            "|----------|-------------|------------|--------|",
        ]
        for t in sorted(found, key=lambda x: PRIORITY_ORDER.index(x.priority)):
            c = t.candidates[0] if t.candidates else None
            conf  = f"{c.confidence}%" if c else "—"
            meth  = c.method if c else "—"
            lines.append(
                f"| `{t.name}` | `{t.tbc_address}` | {conf} | {meth} |"
            )
    else:
        lines.append("*No functions found at ≥ 85% confidence yet.*")

    lines += [
        "",
        "---",
        "",
        "## Suggested Addresses for Manual Verification",
        "",
        "These candidates were found but have confidence < 85%.  "
        "Verify each one in Ghidra or x32dbg before using.",
        "",
    ]

    for priority in PRIORITY_ORDER:
        group = [t for t in missing if t.priority == priority]
        if not group:
            continue
        lines += [
            f"### {_priority_emoji(priority)} {priority} Priority",
            "",
        ]
        for t in group:
            lines += [
                f"#### `{t.name}`",
                "",
                f"- **WotLK address:** `{t.wotlk_address or 'unknown'}`",
                f"- **Description:** {t.description}",
                "",
            ]
            if t.candidates:
                lines += [
                    "| Candidate Address | Name | Confidence | Method | Notes |",
                    "|-------------------|------|------------|--------|-------|",
                ]
                for c in t.candidates[:8]:
                    lines.append(
                        f"| `{c.address}` | `{c.name}` | {c.confidence}% "
                        f"| {c.method} | {c.notes} |"
                    )
            else:
                lines.append("*No candidates found — manual Ghidra analysis required.*")
            lines.append("")

    lines += [
        "---",
        "",
        "## Still Missing (no candidates found)",
        "",
    ]
    no_candidates = [t for t in missing if not t.candidates]
    if no_candidates:
        for t in no_candidates:
            lines.append(f"- `{t.name}` ({t.priority}) — {t.description}")
    else:
        lines.append("*All functions have at least one candidate address.*")

    lines += [
        "",
        "---",
        "",
        "## Next Steps",
        "",
        "1. For each **CRITICAL** function, open Ghidra and verify the top candidate.",
        "2. Update `Docs/symbols/address_map.json` with verified addresses.",
        "3. Replace `0x000000 /* TODO_TBC */` in source files with the verified address.",
        "4. Re-run `python Tools/find_addresses.py` to update this report.",
        "5. See `Docs/GHIDRA_ADDRESS_FINDING.md` for detailed analysis techniques.",
        "",
    ]

    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text("\n".join(lines), encoding="utf-8")
    print(f"[INFO] Report written to {output_path}")


def generate_all_possibilities_report(placeholders: List[SourcePlaceholder],
                                      output_path: Path) -> None:
    lines: List[str] = [
        "# TBC Address All Possibilities",
        "",
        "> Auto-generated by `Tools/find_addresses.py`.",
        "> This report scans actual `TODO_TBC` source placeholders and lists",
        "> plausible exported-symbol candidates for each unresolved target.",
        "",
        "## Summary",
        "",
        "| Metric | Value |",
        "|--------|-------|",
        f"| Source placeholders scanned | {len(placeholders)} |",
        f"| With at least one candidate | {sum(1 for p in placeholders if p.candidates)} |",
        f"| Still with no candidate | {sum(1 for p in placeholders if not p.candidates)} |",
        "",
        "---",
        "",
    ]

    current_file = None
    for placeholder in placeholders:
        if current_file != placeholder.file_path:
            current_file = placeholder.file_path
            lines += [
                f"## `{placeholder.file_path.relative_to(REPO_ROOT).as_posix()}`",
                "",
            ]

        source_ref = placeholder.file_path.relative_to(REPO_ROOT).as_posix()
        lines += [
            f"### `{placeholder.target_name}`",
            "",
            f"- Source: `{source_ref}:{placeholder.line_number}`",
            f"- Comment: {placeholder.comment}",
        ]

        if placeholder.context_function:
            lines.append(f"- Context function: `{placeholder.context_function}`")
        if placeholder.wotlk_address:
            lines.append(f"- WotLK reference: `{placeholder.wotlk_address}`")
        lines.append("")

        if placeholder.candidates:
            lines += [
                "| Candidate Address | Name | Confidence | Method | Notes |",
                "|-------------------|------|------------|--------|-------|",
            ]
            for candidate in placeholder.candidates[:10]:
                lines.append(
                    f"| `{candidate.address}` | `{candidate.name}` | {candidate.confidence}% "
                    f"| {candidate.method} | {candidate.notes} |"
                )
        else:
            lines.append("*No candidates found from current exports.*")

        lines += ["", "---", ""]

    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text("\n".join(lines), encoding="utf-8")
    print(f"[INFO] All-possibilities report written to {output_path}")


# ---------------------------------------------------------------------------
# address_map.json update
# ---------------------------------------------------------------------------

def update_address_map(targets: List[FunctionTarget],
                        map_path: Path) -> None:
    """Merge findings into address_map.json (preserves manual edits)."""
    existing: Dict = {}
    if map_path.exists():
        try:
            existing = json.loads(map_path.read_text(encoding="utf-8"))
        except json.JSONDecodeError:
            pass

    for t in targets:
        entry = existing.get(t.name, {})
        # Never overwrite manually set/verified addresses
        if entry.get("verification_status") == "verified":
            continue
        entry["wotlk_address"]       = t.wotlk_address or "unknown"
        entry["tbc_address"]         = t.tbc_address
        entry["priority"]            = t.priority
        entry["description"]         = t.description
        entry["candidates"]          = [
            {"address": c.address, "name": c.name,
             "confidence": c.confidence, "method": c.method,
             "notes": c.notes}
            for c in t.candidates[:5]
        ]
        entry["confidence"]          = (
            t.candidates[0].confidence if t.candidates else 0
        )
        entry["method"]              = (
            t.candidates[0].method if t.candidates else "none"
        )
        entry.setdefault("verification_status", "pending")
        existing[t.name] = entry

    map_path.parent.mkdir(parents=True, exist_ok=True)
    map_path.write_text(
        json.dumps(existing, indent=2, sort_keys=True),
        encoding="utf-8",
    )
    print(f"[INFO] Address map updated: {map_path}")


# ---------------------------------------------------------------------------
# CLI output
# ---------------------------------------------------------------------------

def print_summary(targets: List[FunctionTarget]) -> None:
    found   = [t for t in targets if t.tbc_address != "TODO_TBC"]
    missing = [t for t in targets if t.tbc_address == "TODO_TBC"]

    print("\n" + "=" * 60)
    print("  TBC Address Finder — Summary")
    print("=" * 60)
    print(f"  Targets   : {len(targets)}")
    print(f"  Found     : {len(found)}")
    print(f"  Missing   : {len(missing)}")
    print()

    for priority in PRIORITY_ORDER:
        group = [t for t in targets if t.priority == priority]
        f_cnt = sum(1 for t in group if t.tbc_address != "TODO_TBC")
        print(f"  {_priority_emoji(priority)} {priority:8s}  {f_cnt}/{len(group)} found")

    if found:
        print("\n  ── HIGH-CONFIDENCE HITS ──────────────────────────────")
        for t in found:
            c = t.candidates[0]
            print(f"  {t.name}")
            print(f"    Address : {t.tbc_address}  ({c.confidence}%)")
            print(f"    Method  : {c.method}")

    print("\n  ── TOP CANDIDATES (unconfirmed) ──────────────────────")
    for priority in PRIORITY_ORDER:
        group = [t for t in missing if t.priority == priority and t.candidates]
        for t in group:
            c = t.candidates[0]
            print(f"  {t.name}  [{priority}]")
            print(f"    Best candidate : {c.address}  ({c.confidence}%)")
            print(f"    Method         : {c.method}")
    print()


# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------

def main() -> int:
    parser = argparse.ArgumentParser(
        description="Search TBC symbol files for TODO_TBC function addresses."
    )
    parser.add_argument("--verbose", "-v", action="store_true",
                        help="Print per-term search details.")
    parser.add_argument("--output", "-o", default=str(OUTPUT_MD),
                        help=f"Path for the findings report (default: {OUTPUT_MD})")
    parser.add_argument("--all-output", default=str(ALL_OUTPUT_MD),
                        help=f"Path for the exhaustive source-placeholder report "
                             f"(default: {ALL_OUTPUT_MD})")
    args = parser.parse_args()

    output_path = Path(args.output)
    all_output_path = Path(args.all_output)

    print(f"[INFO] Loading symbol files …")
    named    = load_sym_file(FUNC_SYM)
    all_syms = load_sym_file(WOW_TBC_SYM)
    functions_csv = load_functions_csv(FUNCTIONS_CSV)
    print(f"[INFO]   func.sym    : {len(named):,} symbols")
    print(f"[INFO]   wow_tbc.sym : {len(all_syms):,} symbols")
    print(f"[INFO]   Functions.csv: {len(functions_csv):,} functions")

    print("[INFO] Searching for addresses …")
    for target in FUNCTION_TARGETS:
        if args.verbose:
            print(f"\n[SEARCH] {target.name}")
        find_function(target, named, all_syms, functions_csv, verbose=args.verbose)

    print_summary(FUNCTION_TARGETS)
    generate_report(FUNCTION_TARGETS, output_path)
    update_address_map(FUNCTION_TARGETS, ADDRESS_MAP_JSON)

    print("[INFO] Scanning source placeholders …")
    placeholders = scan_source_placeholders(SOURCE_ROOT)
    for placeholder in placeholders:
        find_placeholder_candidates(placeholder, named, all_syms, functions_csv)
    generate_all_possibilities_report(placeholders, all_output_path)

    return 0


if __name__ == "__main__":
    sys.exit(main())
