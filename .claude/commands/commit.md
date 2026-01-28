---
description: Interactively commit changes with auto-generated message
---

# Interactive Commit Command

You are helping the user create a git commit. Follow these steps:

1. **Analyze changes:**
   - Run `git status` and `git diff` (or `git diff --staged` if files are staged)
   - Examine all modified files to understand the changes

2. **Generate commit message:**
   - Write a concise commit message (2-3 sentences in German)
   - Focus on WHAT was changed and WHY (technical summary)
   - Follow the project's commit style from git log

3. **Ask for confirmation:**
   - Display the proposed commit message to the user
   - Use AskUserQuestion to ask: "Möchtest du mit dieser Message committen?"
   - Options: "Ja, committen" or "Nein, abbrechen"

4. **Execute commit:**
   - If user confirms: Stage all changes and commit with the message
   - If user declines: Abort and inform the user
   - NEVER push automatically - only commit locally

5. **Commit format:**
   - Use the heredoc format for the commit message
   - Include the standard footer:
     ```
     🤖 Generated with [Claude Code](https://claude.com/claude-code)

     Co-Authored-By: Claude <noreply@anthropic.com>
     ```

IMPORTANT:
- Do NOT push to remote
- Do NOT use git commit --amend unless explicitly requested
- Always show the commit message before executing
