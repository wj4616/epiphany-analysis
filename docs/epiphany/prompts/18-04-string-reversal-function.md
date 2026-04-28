<prompt>
<meta source="prompt-cog"/>

<role>
You are an expert software engineer with deep knowledge of string manipulation, algorithmic efficiency, and language-agnostic edge-case handling. You produce production-quality implementations that are correct, efficient, and well-documented.
</role>

<context>
The target audience is a developer seeking a production-quality string reversal utility.
</context>

<task>
## Purpose
Implement a function that reverses a string — taking a string as input and returning its characters in reverse order.

## Reasoning Before Implementation
Before writing code, reason through the following steps:
1. **Algorithm choice**: Consider available approaches (two-pointer swap, stack-based, recursive, slice-based) and select the most appropriate one, noting any trade-offs.
2. **Language-specific string handling**: Account for whether strings are mutable or immutable in the target language, and how that affects the reversal approach.
3. **Implementation**: Write the function based on the reasoning above.

## Requirements
- Accept a string input and return the reversed string.
- Handle all character encodings correctly (see edge cases and constraints).
- Include inline comments explaining non-obvious logic.
- Specify time and space complexity.
</task>

<constraints>
- DO handle Unicode and multibyte characters correctly.
- DO NOT use built-in reverse methods unless you explicitly note the trade-offs of doing so.
- DO specify time and space complexity for the chosen algorithm.
- DO provide example test cases demonstrating correct behavior.
- DO NOT assume ASCII-only input.
</constraints>

<output_format>
Provide your response in the following structure:

1. **Function Signature** — The function name, parameters, and return type.
2. **Implementation** — The complete function code with inline comments explaining key logic.
3. **Complexity Note** — A brief statement of time and space complexity (e.g., O(n) time, O(n) space).
4. **Test Cases** — 3–5 example test cases covering normal and edge-case inputs, with expected outputs.
</output_format>

<edge_cases>
- **Empty string**: `""` → `""` (returns empty string).
- **Single character**: `"a"` → `"a"` (returns itself).
- **Unicode/multibyte characters**: Characters such as emoji, accented characters, or combining character sequences must be reversed at the grapheme or code-point level, not at the byte level. For example, `"café"` → `"éfac"`, not a corrupted byte sequence.
- **Null/undefined input**: Define explicit behavior — either return an empty string, throw an error, or return null/undefined. State which behavior the function adopts.
</edge_cases>
</prompt>