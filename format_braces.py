from pathlib import Path
import re
root = Path(r'd:\Alex\dev\repo\datacollector')
paths = list(root.glob('src/**/*.[hc]pp')) + list(root.glob('tests/**/*.[hc]pp')) + [root / 'src' / 'main.cpp']
patterns = [
    (r'namespace\\s+([^\\s{]+)\\s*\\{', r'namespace \\1\\n{'),
    (r'class\\s+([^\\s{]+)\\s*\\{', r'class \\1\\n{'),
    (r'struct\\s+([^\\s{]+)\\s*\\{', r'struct \\1\\n{'),
    (r'template\\s*<([^>]+)>\\s*\\{', r'template<\\1>\\n{'),
    (r'\\)\\s*\\{', r')\\n{'),
    (r'if\\s*\\(([^)]+)\\)\\s*\\{', r'if (\\1)\\n{'),
    (r'for\\s*\\(([^)]+)\\)\\s*\\{', r'for (\\1)\\n{'),
    (r'while\\s*\\(([^)]+)\\)\\s*\\{', r'while (\\1)\\n{'),
    (r'else\\s*\\{', r'else\\n{'),
    (r'switch\\s*\\(([^)]+)\\)\\s*\\{', r'switch (\\1)\\n{'),
]
for path in paths:
    if not path.is_file():
        continue
    text = path.read_text(encoding='utf-8')
    orig = text
    for pat, rep in patterns:
        text = re.sub(pat, rep, text)
    if text != orig:
        path.write_text(text, encoding='utf-8')
        print('updated', path)
