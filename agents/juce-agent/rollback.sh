#!/bin/bash
# rollback.sh - Restore playbookdata from backup

set -e

if [ -z "$1" ]; then
    echo "Usage: ./rollback.sh <backup-dir>"
    echo ""
    echo "Available backups:"
    ls -d playbookdata-backup-* 2>/dev/null || echo "  (none found)"
    exit 1
fi

BACKUP_DIR="$1"

if [ ! -d "$BACKUP_DIR" ]; then
    echo "Error: Backup directory not found: $BACKUP_DIR"
    exit 1
fi

echo "Restoring from: $BACKUP_DIR"

# Remove current playbookdata
rm -rf playbookdata

# Copy backup
cp -r "$BACKUP_DIR" playbookdata

# Clear Python cache
find . -type d -name __pycache__ -exec rm -rf {} + 2>/dev/null || true

echo "✓ Rollback complete"
echo "Restored: $(find playbookdata -name '*.json' | wc -l) JSON files"