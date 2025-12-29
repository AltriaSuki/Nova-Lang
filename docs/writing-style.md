# Documentation Style Guide

This repository maintains design documents and draft specifications for the Nova language and its compiler implementation. To ensure that documentation remains readable, reviewable, and testable, documents should follow the conventions below.

## 1. Tone and Register

- Use a formal, technical register; avoid colloquial phrasing (e.g., informal headings and idioms).
- Avoid contractions in prose; prefer full forms (e.g., “does not” / “do not”).
- Prefer an impersonal style when appropriate (e.g., “the compiler” / “the implementation” / “this document”), and avoid second-person address in specification text.
- Prefer precise statements of scope and intent (goals, non-goals, assumptions, constraints).
- When describing an incomplete implementation, use “placeholder”, “scaffold”, or “not yet implemented”.

## 2. Recommendations vs. Requirements

This repository generally avoids RFC-style normative keywords. Prefer descriptive statements (e.g., “the compiler rejects …”) and standard modal verbs (e.g., “should”, “may”) when describing intended behavior.

## 3. Status Labels

Each major document should declare a status near the top, for example:
- **Status:** Draft
- **Status:** Informative (non-normative)
- **Status:** Historical (superseded)

## 4. Cross-References

- Prefer stable, repository-relative paths (e.g., `docs/language-spec.md`).
- Ensure that link text remains descriptive and consistent.
