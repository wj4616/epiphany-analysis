#!/usr/bin/env python3
"""Test harvest with API key."""
import os
import sys

# Set BEFORE any other imports
os.environ['FIRECRAWL_API_KEY'] = 'fc-47b7637dde8645afb12b77cf11fd79ec'

from pathlib import Path

# Add scripts to path
script_dir = Path(__file__).parent
sys.path.insert(0, str(script_dir))

# Now read the module globals after setting env
import importlib.util
spec = importlib.util.spec_from_file_location("harvest_deep", script_dir / "harvest-deep.py")
harvest_deep = importlib.util.module_from_spec(spec)
spec.loader.exec_module(harvest_deep)

print('API Key:', harvest_deep.FIRECRAWL_API_KEY[:15] + '...')

# Create client
client = harvest_deep.FirecrawlClient(harvest_deep.FIRECRAWL_API_KEY, harvest_deep.FIRECRAWL_API_URL)

# Test search
result = client.search('Freeverb', limit=1)
print('Success:', result.get('success'))
print('URL:', result.get('data', [{}])[0].get('url', 'N/A') if result.get('data') else 'No results')