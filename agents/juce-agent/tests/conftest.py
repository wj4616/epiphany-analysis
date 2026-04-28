# tests/conftest.py
import sys
from pathlib import Path

# Add playbookdata/scripts to path for imports
scripts_path = Path(__file__).parent.parent / "playbookdata" / "scripts"
if str(scripts_path) not in sys.path:
    sys.path.insert(0, str(scripts_path))