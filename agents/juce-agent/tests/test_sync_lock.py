# tests/test_sync_lock.py
import pytest
import os
import sys
from pathlib import Path
import tempfile
import time

# Add playbookdata/scripts to path BEFORE importing
sys.path.insert(0, str(Path(__file__).parent.parent / "playbookdata" / "scripts"))

from sync_lock import SyncLock, LockTimeoutError, MultiLock, acquire_kb_lock

def test_acquire_lock_creates_lock_file():
    """Lock acquisition creates lock file with PID."""
    with tempfile.TemporaryDirectory() as tmpdir:
        lock = SyncLock("test-lock", timeout=5, locks_dir=Path(tmpdir))
        lock.acquire()

        lock_file = Path(tmpdir) / "test-lock.lock"
        assert lock_file.exists()

        content = lock_file.read_text()
        assert str(os.getpid()) in content

        lock.release()
        assert not lock_file.exists()

def test_acquire_lock_timeout_raises_error():
    """Lock acquisition times out if lock is held by another fcntl lock."""
    with tempfile.TemporaryDirectory() as tmpdir:
        locks_dir = Path(tmpdir)

        # Create and hold a lock using fcntl
        import fcntl
        lock_file = locks_dir / "held.lock"
        lock_file.write_text(str(os.getpid()))

        # Open and lock the file to simulate another process holding it
        fd = open(lock_file, 'w')
        fcntl.flock(fd.fileno(), fcntl.LOCK_EX | fcntl.LOCK_NB)

        try:
            # Try to acquire same lock with short timeout
            lock = SyncLock("held", timeout=1, locks_dir=locks_dir)

            with pytest.raises(LockTimeoutError):
                lock.acquire()
        finally:
            # Release our lock
            fcntl.flock(fd.fileno(), fcntl.LOCK_UN)
            fd.close()

def test_stale_lock_is_removed():
    """Stale lock (dead process) is removed and acquired."""
    with tempfile.TemporaryDirectory() as tmpdir:
        locks_dir = Path(tmpdir)

        # Create lock file (simulating stale lock)
        # Note: fcntl will succeed on stale lock since no process holds it
        lock_file = locks_dir / "stale.lock"
        lock_file.write_text("99999999")  # Fake PID

        lock = SyncLock("stale", timeout=5, locks_dir=locks_dir)
        # This should succeed because fcntl sees no actual lock holder
        lock.acquire()

        # Lock file now has our PID
        assert lock_file.exists()
        assert str(os.getpid()) in lock_file.read_text()

        lock.release()

def test_context_manager_releases_on_exception():
    """Lock is released even if exception occurs."""
    with tempfile.TemporaryDirectory() as tmpdir:
        locks_dir = Path(tmpdir)
        lock_file = locks_dir / "test.lock"

        try:
            with SyncLock("test", timeout=5, locks_dir=locks_dir):
                raise ValueError("Test error")
        except ValueError:
            pass

        assert not lock_file.exists()