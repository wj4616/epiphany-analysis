#!/usr/bin/env python3
"""
File locking for KB sync operations.

Provides atomic lock acquisition using fcntl on POSIX systems.
Extends the pattern from harvest-deep.py with multi-lock support.
"""

import os
import sys
import fcntl
import time
from pathlib import Path
from typing import Optional

# Add scripts directory to path for local imports
sys.path.insert(0, str(Path(__file__).parent))

# Default lock directory
DEFAULT_LOCKS_DIR = Path(__file__).parent.parent / "harvested" / "locks"


class LockTimeoutError(Exception):
    """Raised when lock cannot be acquired within timeout."""
    pass


class SyncLock:
    """
    Atomic file lock using fcntl for POSIX systems.

    Usage:
        lock = SyncLock("dsp-kb")
        lock.acquire()
        try:
            # ... do work ...
        finally:
            lock.release()

        # Or use context manager:
        with SyncLock("dsp-kb") as lock:
            # ... do work ...
    """

    def __init__(self, name: str, timeout: int = 30, locks_dir: Path = None):
        """
        Initialize lock.

        Args:
            name: Lock name (e.g., "dsp-kb", "master-index")
            timeout: Maximum seconds to wait for lock
            locks_dir: Directory for lock files
        """
        self.name = name
        self.timeout = timeout
        self.locks_dir = locks_dir or DEFAULT_LOCKS_DIR
        self.lock_path = self.locks_dir / f"{name}.lock"
        self.fd = None

    def acquire(self) -> "SyncLock":
        """
        Atomically acquire lock.

        Raises:
            LockTimeoutError: If lock cannot be acquired within timeout
        """
        # Ensure directory exists
        self.locks_dir.mkdir(parents=True, exist_ok=True)

        start = time.time()

        while time.time() - start < self.timeout:
            try:
                # Try to create and lock the file
                self.fd = open(self.lock_path, 'w')
                fcntl.flock(self.fd.fileno(), fcntl.LOCK_EX | fcntl.LOCK_NB)
                self.fd.write(f"{os.getpid()}\n")
                self.fd.flush()
                return self

            except (IOError, OSError):
                # Lock held by another process
                if self.fd:
                    try:
                        self.fd.close()
                    except:
                        pass
                    self.fd = None
                time.sleep(0.1)

        raise LockTimeoutError(f"Could not acquire lock: {self.lock_path}")

    def release(self):
        """Release lock."""
        if self.fd:
            try:
                fcntl.flock(self.fd.fileno(), fcntl.LOCK_UN)
                self.fd.close()
            except:
                pass
            self.fd = None

        if self.lock_path.exists():
            try:
                self.lock_path.unlink()
            except:
                pass

    def __enter__(self):
        self.acquire()
        return self

    def __exit__(self, *args):
        self.release()


class MultiLock:
    """
    Acquire multiple locks atomically.

    Usage:
        locks = MultiLock([SyncLock("harvest"), SyncLock("dsp-kb")])
        locks.acquire()
        try:
            # ... do work ...
        finally:
            locks.release()
    """

    def __init__(self, locks: list[SyncLock]):
        self.locks = locks

    def acquire(self) -> "MultiLock":
        """Acquire all locks in order."""
        acquired = []
        try:
            for lock in self.locks:
                lock.acquire()
                acquired.append(lock)
            return self
        except:
            # Release any acquired locks on failure
            for lock in reversed(acquired):
                lock.release()
            raise

    def release(self):
        """Release all locks in reverse order."""
        for lock in reversed(self.locks):
            lock.release()

    def __enter__(self):
        self.acquire()
        return self

    def __exit__(self, *args):
        self.release()


def is_process_alive(pid: int) -> bool:
    """
    Check if process is running. Platform-specific.

    Args:
        pid: Process ID to check

    Returns:
        True if process is running
    """
    if os.name == 'posix':
        # Linux: check /proc/{pid}
        if Path(f'/proc/{pid}').exists():
            return True
        # macOS/BSD: use kill(pid, 0)
        try:
            os.kill(pid, 0)
            return True
        except OSError:
            return False
    else:
        # Windows: not supported, assume alive
        return True


def acquire_kb_lock(kb: str, timeout: int = 30) -> MultiLock:
    """
    Acquire locks for KB sync operation.

    Acquires both harvest lock (prevents concurrent harvest) and KB lock.

    Args:
        kb: KB name (e.g., "dsp-kb")
        timeout: Maximum seconds to wait

    Returns:
        MultiLock that must be released after use
    """
    harvest_lock = SyncLock("harvest", timeout)
    kb_lock = SyncLock(kb, timeout)
    return MultiLock([harvest_lock, kb_lock])


if __name__ == "__main__":
    import sys

    if len(sys.argv) < 2:
        print("Usage: sync-lock.py <lock_name> [--release]")
        sys.exit(1)

    name = sys.argv[1]

    if "--release" in sys.argv:
        lock_file = DEFAULT_LOCKS_DIR / f"{name}.lock"
        if lock_file.exists():
            lock_file.unlink()
            print(f"Released: {name}")
        else:
            print(f"Lock not found: {name}")
    else:
        print(f"Acquiring lock: {name}")
        with SyncLock(name, timeout=5) as lock:
            print(f"Lock acquired: {name}")
            input("Press Enter to release...")
        print(f"Lock released: {name}")