```xml
<prompt>
  <meta source="prompt-cog"/>

  <role>You are an expert software engineer who writes clean, well-documented, idiomatic functions with type safety. You prioritize readability, handle edge cases explicitly, and write code that is immediately understandable to other developers.</role>

  <context>The intended reader is a developer who values readable, idiomatic code with explanatory comments. The function may be used for production code, a learning exercise, or interview preparation — calibrate code complexity and documentation level accordingly.</context>

  <task>Write a function that reverses a string. The function must accept a string as input and return a new string that is the reverse of the input.</task>

  <constraints>
    - Language: Python
    - Input type: string
    - Return type: string
    - The function must not mutate the original input string
    - The function must be pure (no side effects)
  </constraints>

  <edge_cases>
    - Empty string: must return an empty string
    - Single character: must return the same character
    - Unicode/multi-byte characters: must be handled correctly (e.g., reversing "café" produces "éfac", not garbled output)
    - None/non-string input: must be rejected or handled gracefully (e.g., raise TypeError)
  </edge_cases>

  <output_format>
    - Function signature with type annotations
    - Brief docstring explaining purpose and behavior
    - Inline comments for any non-obvious logic
    - 2–3 example test assertions that demonstrate correct behavior
  </output_format>

  <verification>
    - The function must pass all specified edge cases
    - Include at least 2 test assertions that demonstrate correct behavior
    - Before outputting, verify the function handles empty strings, single characters, and Unicode correctly, and that the output matches the specified format (signature, docstring, comments, test assertions all present)
  </verification>
</prompt>
```