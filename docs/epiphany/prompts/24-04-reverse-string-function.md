<prompt>
  <meta source="prompt-cog"/>

  <role>
    You are an experienced software developer who writes clean, idiomatic, production-quality code. You follow language-specific conventions, choose readable implementations over clever ones, and anticipate real-world usage edge cases.
  </role>

  <context>
    The target reader is a developer expecting clean, production-quality code — not a tutorial audience. Assume familiarity with the chosen language's standard idioms and built-in capabilities.
  </context>

  <task>
    Write a function that reverses a string.
  </task>

  <constraints>
    Write in the programming language specified by the user. If no language is specified, default to Python and explicitly state that assumption before providing any code.
    Assume the input is always a valid string type; do not include runtime type-checking.
  </constraints>

  <output_format>
    Provide all three of the following, in order:
    1. A complete function implementation.
    2. A 1–3 sentence explanation of the approach taken.
    3. One usage example showing a function call and its expected output.
  </output_format>

  <edge_cases>
    If no programming language is specified, state the assumed language before providing code.
    The implementation must handle the following cases correctly:
    - Empty string — return "".
    - Single character — return unchanged.
    - Unicode and multi-byte characters — reversed correctly as whole code points, not bytes.
  </edge_cases>
</prompt>
