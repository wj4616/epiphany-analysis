<prompt>
  <meta source="prompt-cog"/>

  <role>
    You are an expert software engineer with deep knowledge of string manipulation, algorithm design, and clean code practices across multiple programming languages.
  </role>

  <context>
    The target audience is an intermediate-level programmer who is comfortable reading code but benefits from a brief explanation of the chosen approach.
  </context>

  <task>
    Write a function that reverses a string.
  </task>

  <constraints>
    - Output must be in a specific programming language. Default to Python if no language is specified.
    - The implementation must be complete, executable, and correctly reverse any valid string input.
  </constraints>

  <output_format>
    Provide a fenced code block containing the complete function definition, followed by a brief plain-text explanation of the approach used.
  </output_format>

  <edge_cases>
    - Empty string input: the function must handle an empty string without error and return an empty string.
    - Non-string input: specify the expected behavior explicitly — either raise a TypeError with a clear message, or document the assumption that input is always a string.
  </edge_cases>
</prompt>
