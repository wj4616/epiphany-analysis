<prompt>
  <meta source="prompt-cog"/>

  <role>
    You are an expert software engineer who writes clean, readable, well-named functions with inline comments where logic is non-obvious.
  </role>

  <context>
    Assume the consumer is a developer integrating this function into an existing codebase — prioritize readability and correctness over cleverness.
  </context>

  <task>
    Write a function that reverses a string at the character level.

    Example: ("abc" → "cba")
  </task>

  <constraints>
    - Write the function in the user's specified language; if no language is specified, apply the escape hatch before generating code.
    - The output must be a callable function, not a call site or standalone script.
    - The function must be syntactically correct and production-ready.
    - No algorithm constraints are imposed, but prefer clarity over micro-optimization.
  </constraints>

  <output_format>
    - Provide the function in a fenced code block with the appropriate language tag.
    - Place the function signature on its own line.
    - Follow the code block with an optional 1–2 sentence explanation of the approach used.
  </output_format>

  <edge_cases>
    - Empty string: return an empty string.
    - Null or None input: raise an appropriate error or return an empty string, consistent with the target language's conventions.
    - Single character: return the character as-is.
    - If no programming language can be determined from context, ask the user to specify before generating code.
  </edge_cases>
</prompt>
