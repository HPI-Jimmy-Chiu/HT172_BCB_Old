# -*- coding: utf-8 -*-
"""
Big5-safe patch tool for HT172 source files.
Usage: python big5_patch.py <file> <old_pattern> <new_pattern>
   or: import and use patch_file() / patch_file_from_config()

All file I/O uses binary mode (rb/wb) to preserve Big5 encoding.
JimmyChiu 20260416
"""
import sys
import json
import os


def patch_file(filepath, replacements):
    """
    Apply replacements to a Big5-encoded file using raw bytes.

    Args:
        filepath: Path to the Big5 source file
        replacements: List of (old, new, description) tuples.
                      old/new can be str (ASCII only) or bytes.
    Returns:
        List of (description, success_bool) results
    """
    with open(filepath, 'rb') as f:
        data = f.read()

    results = []
    for old, new, desc in replacements:
        old_bytes = old.encode('ascii') if isinstance(old, str) else old
        new_bytes = new.encode('ascii') if isinstance(new, str) else new
        count = data.count(old_bytes)
        if count == 0:
            print(f"  [SKIP] '{desc}' - pattern not found")
            results.append((desc, False))
            continue
        if count > 1:
            print(f"  [WARN] '{desc}' - found {count} times, replacing first only")
        data = data.replace(old_bytes, new_bytes, 1)
        print(f"  [OK]   {desc}")
        results.append((desc, True))

    with open(filepath, 'wb') as f:
        f.write(data)
    print(f"  Saved: {filepath}\n")
    return results


def patch_file_from_config(config_path):
    """
    Apply patches from a JSON config file.

    JSON format:
    {
      "patches": [
        {
          "file": "path/to/file.cpp",
          "replacements": [
            {
              "old": "original text (ASCII)",
              "new": "replacement text (ASCII)",
              "desc": "short description"
            }
          ]
        }
      ]
    }
    """
    with open(config_path, 'r', encoding='utf-8') as f:
        config = json.load(f)

    all_results = []
    for entry in config['patches']:
        filepath = entry['file']
        print(f"=== Patching {os.path.basename(filepath)} ===")
        replacements = [
            (r['old'], r['new'], r['desc'])
            for r in entry['replacements']
        ]
        results = patch_file(filepath, replacements)
        all_results.extend(results)

    ok = sum(1 for _, s in all_results if s)
    fail = sum(1 for _, s in all_results if not s)
    print(f"=== Done: {ok} applied, {fail} skipped ===")
    return all_results


def main():
    if len(sys.argv) == 2 and sys.argv[1].endswith('.json'):
        # JSON config mode
        patch_file_from_config(sys.argv[1])
    elif len(sys.argv) == 4:
        # Inline mode: file old new
        filepath, old, new = sys.argv[1], sys.argv[2], sys.argv[3]
        patch_file(filepath, [(old, new, f"{old[:30]}... -> {new[:30]}...")])
    else:
        print("Usage:")
        print("  python big5_patch.py <config.json>")
        print("  python big5_patch.py <file> <old_text> <new_text>")
        print()
        print("JSON config example:")
        print('  { "patches": [{ "file": "path.cpp",')
        print('    "replacements": [{"old":"x","new":"y","desc":"change x to y"}] }] }')
        sys.exit(1)


if __name__ == '__main__':
    main()
