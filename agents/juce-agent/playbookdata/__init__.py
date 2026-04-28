"""
Playbook Data Package

Knowledge base system for JUCE VST plugin development.

Usage:
    from playbookdata import get_kb

    kb = get_kb()
    results = kb.query_topic("dsp")
"""

from .kb_lookup import (
    get_kb,
    reset_kb,
    KBQuery,
    KBResult,
    KBFile,
    KBError,
    KBNotFoundError,
    KBIntegrityError,
)

__all__ = [
    'get_kb',
    'reset_kb',
    'KBQuery',
    'KBResult',
    'KBFile',
    'KBError',
    'KBNotFoundError',
    'KBIntegrityError',
]

__version__ = '1.0.0'