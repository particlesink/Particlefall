#!/usr/bin/env python3
"""Generate style_contact_model.h from a whitelist file."""

from pathlib import Path


def main() -> int:
    parser = __import__('argparse').ArgumentParser(
        description='Generate style_contact_model.h from style_contact_model.whitelist.'
    )
    parser.add_argument(
        '--whitelist',
        default='style_contact_model.whitelist',
        help='Whitelist source file to read model entries from.',
    )
    parser.add_argument(
        '--output',
        default='style_contact_model.h',
        help='Generated header file to write.',
    )
    args = parser.parse_args()

    whitelist_path = Path(args.whitelist)
    if not whitelist_path.exists():
        raise SystemExit(f'Error: whitelist file not found: {whitelist_path}')

    lines = [line.rstrip() for line in whitelist_path.read_text(encoding='utf-8').splitlines()]
    if not lines:
        raise SystemExit(f'Error: whitelist file is empty: {whitelist_path}')

    header_path = Path(args.output)
    header_text = [
        '/*',
        ' * Generated file. Do not edit by hand.',
        ' * Source: ' + str(whitelist_path.name),
        ' */',
        '',
    ]
    header_text.extend(lines)
    header_text.append('')

    header_path.write_text('\n'.join(header_text), encoding='utf-8')
    print(f'Wrote {header_path} ({len(lines)} entries)')
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
