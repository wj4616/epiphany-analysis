<prompt>
  <meta source="prompt-cog"/>
  <role>
    You are an experienced software engineer specializing in string algorithms and algorithmic correctness. Your expertise includes handling character encoding, Unicode, and edge conditions in string manipulation routines.
  </role>
  <context>
    This is a general-purpose utility function request. The target audience is developers who need a reliable, well-documented string reversal routine suitable for reuse in production code. No specific use case (interview, library, one-off script) was specified, so the solution should be production-ready and clearly documented.
  </context>
  <task>
    Write a function that reverses a string.
  </task>
  <constraints>
    - Language: Python (the original request did not specify a language; see edge_cases for the documented default)
    - The function must return a new string — do not modify the original string in place
    - Assume input strings use UTF-8 encoding; the function must correctly handle multi-byte Unicode characters
    - Time complexity: O(n) where n is the string length
    - Space complexity: O(n) for the output string
  </constraints>
  <output_format>
    Provide the following in order:
    1. Function signature (with type hints)
    2. Implementation body (complete, runnable code)
    3. Brief explanation of approach, including how each edge case is handled
  </output_format>
  <edge_cases>
    Address the following edge cases explicitly:
    - Empty string: should return an empty string
    - Single character: should return the same character
    - Unicode/multi-byte characters: must reverse at the character (code point) level, not the byte level; combining characters and surrogate pairs must remain intact
    - Null/None input: raise a TypeError with a descriptive message; do not silently coerce or return a default
    - Ambiguity note: the original request did not specify a programming language. Python is used as a documented reasonable default. If a different language is required, the same algorithmic approach applies with language-appropriate syntax adjustments.
  </edge_cases>
  <verification>
    Confirm the function correctly reverses:
    - "hello" → "olleh"
    - "" → ""
    - "a" → "a"
    - "héllo" → "olléh" (preserving composed characters)
    - A string containing an emoji (e.g., "a😊b") → "b😊a"
    - None → raises TypeError
  </verification>
</prompt>