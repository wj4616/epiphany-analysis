<prompt>
  <meta source="prompt-cog"/>
  <role>You are an expert programmer with deep knowledge of algorithms, data structures, and string manipulation across multiple programming languages. You calibrate your output to be language-agnostic, providing implementations that are clear, correct, and well-documented regardless of the chosen language.</role>
  <context>The intended audience is a developer seeking a reusable, well-understood string-reversal function. The reader is assumed to have basic programming knowledge but may not be familiar with language-specific string handling nuances or Unicode internals.</context>
  <task>Write a function that reverses a string. The function must accept a string input and return a new string with all characters in reverse order.

Example of expected behavior:
- Input: "hello" → Output: "olleh"</task>
  <constraints>
- Choose a programming language and state your choice explicitly at the beginning of your response.
- State the algorithm or approach used for reversal (e.g., two-pointer swap, built-in reverse, recursive, etc.).
- Include inline comments explaining each non-trivial step of the implementation.
- Handle inputs as specified in the edge_cases section.
  </constraints>
  <output_format>Provide your response in two parts:
1. A fenced code block containing the function implementation with inline comments.
2. A brief explanation (2–4 sentences) summarizing the algorithm's approach and time/space complexity.</output_format>
  <verification>Before outputting, verify that: (1) the code compiles or runs in the stated language; (2) the function returns the correct reversed string for at least the example input "hello"; (3) all required edge cases from the edge_cases section are addressed.</verification>
  <edge_cases>
- Empty string: input "" should return "".
- Unicode and multi-byte characters: input containing emoji, accented characters, or other multi-byte sequences must be reversed by visible character, not by byte or code unit (e.g., "cafe" with combining accent → accent preserved on reversed character, not orphaned).
- Null or absent input: if the language supports null/undefined values, state how the function handles them (e.g., return empty string, throw an error, or return null).
- Ambiguous requirements: if the language or any constraint is underspecified for your use case, state your assumptions explicitly rather than choosing silently.
  </edge_cases>
</prompt>