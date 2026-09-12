#!/usr/bin/env python3
"""Check implemented plumbing callees against declared source files, not call graphs."""
import pathlib
import re
import sys
root = pathlib.Path(__file__).resolve().parents[1]
errors = []
checked = 0
for kind in ("feature", "functionality"):
    for path in sorted((root / "src/blueprint" / kind).glob("*.md")):
        for line in path.read_text().splitlines():
            cells = [cell.strip().strip("`") for cell in line.strip().strip("|").split("|")]
            if len(cells) != 7 or cells[6] != "Implemented" or not cells[0].isdigit():
                continue
            _, caller, callee, filename, *_ = cells
            source = root / filename
            if not source.is_file():
                errors.append(f"{path.name}: missing {filename}")
                continue
            text = source.read_text()
            if source.suffix == ".h" and "::" in callee:
                owner, symbol = callee.rsplit("::", 1)
                found = re.search(r"\b(class|struct)\s+" + re.escape(owner) + r"\b", text)
                found = found and re.search(r"\b" + re.escape(symbol) + r"\s*\(", text)
            else:
                found = re.search(r"\b" + re.escape(callee) + r"\s*\(", text)
            if not found:
                errors.append(f"{path.name}: {callee} not found in {filename}")
            checked += 1
if errors:
    print("\n".join(errors), file=sys.stderr)
    sys.exit(1)
print(f"OK: {checked} implemented plumbing callees found; caller semantics require review")
