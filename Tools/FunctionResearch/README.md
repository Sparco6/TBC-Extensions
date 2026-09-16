# Function Research Tool

This tool enriches a copy of the Ghidra function index. It never modifies the
input `Functions.csv` or symbol files.

```powershell
py -3 enrich_functions.py ..\..\..\Functions.csv `
  --symbols ..\..\..\wow_tbc.sym ..\..\..\wow_tbcy.sym `
  --offsets ..\..\TBCExtensions\src\Offsets\ClientOffsets_8606.hpp `
  --research research_export.csv --output output
```

`--research` is optional and accepts the CSV columns exported by MasterWoW
DevTools. Outputs are enriched, unknown, verified, category-sorted CSV files
plus `statistics.json`.
