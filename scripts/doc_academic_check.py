#!/usr/bin/env python3
from __future__ import annotations

import argparse
import pathlib
import re
import sys
from dataclasses import dataclass


FENCE_RE = re.compile(r"^```")
INLINE_CODE_RE = re.compile(r"`[^`]*`")
MD_LINK_RE = re.compile(r"\[([^\]]+)\]\(([^)]+)\)")

SENTENCE_RE = re.compile(r"(?<=[.!?])\s+")

CONTRACTION_RE = re.compile(
    r"\b(?:[A-Za-z]+n['’]t|I['’]m|you['’]re|we['’]re|it['’]s|that['’]s|there['’]s|won['’]t|can['’]t)\b"
)
NORMATIVE_UPPER_RE = re.compile(r"\b(?:MUST|MUST NOT|SHOULD|SHOULD NOT|MAY)\b")
TODO_RE = re.compile(r"\b(?:TODO|TBD|FIXME|XXX)\b")

INFORMAL_RE = re.compile(
    r"\b(?:reality check|wishlist|annoying|kinda|sorta|super|awesome|cool|hacky)\b",
    re.IGNORECASE,
)

# A lightweight, prose-only check; code examples and inline code are excluded by
# stripping fenced blocks and inline code spans.
#
# Notes:
# - Excludes "I" when used as part of "I/O".
PRONOUN_RE = re.compile(
    r"(^|[^A-Za-z])(?:we|you|your|yours|ours|my|our)\b|\bI\b(?!/)",
    re.IGNORECASE,
)


@dataclass(frozen=True)
class Finding:
    path: str
    line: int
    rule: str
    snippet: str


def strip_markdown_prose(text: str) -> str:
    lines = text.splitlines()
    out: list[str] = []
    in_fence = False

    for line in lines:
        if FENCE_RE.match(line.strip()):
            in_fence = not in_fence
            out.append("")  # keep line count stable
            continue
        if in_fence:
            out.append("")
            continue
        # Strip inline code and inline links (keep link text).
        line = INLINE_CODE_RE.sub("", line)
        line = MD_LINK_RE.sub(lambda m: m.group(1), line)
        out.append(line)

    return "\n".join(out)


def iter_sentences_with_line_numbers(text: str) -> list[tuple[int, str]]:
    results: list[tuple[int, str]] = []
    for idx, raw_line in enumerate(text.splitlines(), start=1):
        line = raw_line.strip()
        if not line:
            continue
        # Ignore headings and Markdown structure lines with no prose content.
        if line.startswith(("#", "```", "|", "-", "*")) and len(line) <= 3:
            continue
        for sent in SENTENCE_RE.split(line):
            sent = sent.strip()
            if sent:
                results.append((idx, sent))
    return results


def check_file(path: pathlib.Path) -> tuple[int, list[Finding]]:
    raw = path.read_text(encoding="utf-8")
    prose = strip_markdown_prose(raw)
    sentences = iter_sentences_with_line_numbers(prose)

    findings: list[Finding] = []
    for line_no, sentence in sentences:
        if CONTRACTION_RE.search(sentence):
            findings.append(Finding(str(path), line_no, "contraction", sentence))
        if NORMATIVE_UPPER_RE.search(sentence):
            findings.append(Finding(str(path), line_no, "normative-uppercase", sentence))
        if TODO_RE.search(sentence):
            findings.append(Finding(str(path), line_no, "todo-marker", sentence))
        if INFORMAL_RE.search(sentence):
            findings.append(Finding(str(path), line_no, "informal-wording", sentence))
        if PRONOUN_RE.search(sentence):
            findings.append(Finding(str(path), line_no, "first/second-person", sentence))

    return len(sentences), findings


def main(argv: list[str]) -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--root",
        default=".",
        help="Repository root (default: current directory)",
    )
    parser.add_argument(
        "--include-tests",
        action="store_true",
        help="Include tests/** Markdown files (default: included)",
    )
    args = parser.parse_args(argv)

    root = pathlib.Path(args.root)
    md_files = [
        p
        for p in root.rglob("*.md")
        if ".git" not in p.parts and (args.include_tests or True)
    ]

    md_files = sorted(md_files, key=lambda p: str(p))

    total_sentences = 0
    all_findings: list[Finding] = []
    for path in md_files:
        count, findings = check_file(path)
        total_sentences += count
        all_findings.extend(findings)

    print(f"Scanned {len(md_files)} Markdown files; {total_sentences} prose sentences.")
    if not all_findings:
        print("No findings.")
        return 0

    print(f"Findings: {len(all_findings)}")
    for f in all_findings:
        print(f"- {f.path}:{f.line} [{f.rule}] {f.snippet}")
    return 1


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
