import re

def generate_lexer_switch(def_file_path):
    puncts = []
    # 匹配 NOVA_PUNCT(name, spelling)
    pattern = re.compile(r'NOVA_PUNCT\s*\(\s*(\w+)\s*,\s*"([^"]+)"\s*\)')

    with open(def_file_path, 'r') as f:
        for line in f:
            match = pattern.search(line)
            if match:
                puncts.append({'name': match.group(1), 'spelling': match.group(2)})

    # 按首字母分组
    groups = {}
    for p in puncts:
        first_char = p['spelling'][0]
        groups.setdefault(first_char, []).append(p)

    output = ["switch (c) {"]
    
    for char in sorted(groups.keys()):
        output.append(f"    case '{char}': {{")
        # 同一首字母下，按长度降序排列（先尝试匹配最长的，如 >= 优先于 >）
        sorted_group = sorted(groups[char], key=lambda x: len(x['spelling']), reverse=True)
        
        # 处理多字符情况
        multi = [p for p in sorted_group if len(p['spelling']) > 1]
        single = [p for p in sorted_group if len(p['spelling']) == 1]

        for p in multi:
            rest = p['spelling'][1:]
            # 检查剩余字符是否匹配
            cond = " && ".join([f"buffer_ptr_[{i}] == '{c}'" for i, c in enumerate(rest)])
            output.append(f"        if (buffer_ptr_ + {len(rest)} <= buffer_end_ && {cond}) {{")
            output.append(f"            buffer_ptr_ += {len(rest)};")
            output.append(f"            kind = TokenKind::{p['name']};")
            output.append(f"            break;")
            output.append(f"        }}")
        
        if single:
            output.append(f"        kind = TokenKind::{single[0]['name']};")
        else:
            output.append(f"        kind = TokenKind::unknown;")
            
        output.append("        break;")
        output.append("    }")

    output.append("    default: break;")
    output.append("}")
    return "\n".join(output)

if __name__ == "__main__":
    # 假设你的 .def 文件路径如下
    code = generate_lexer_switch("include/nova/Lex/TokenKinds.def")
    with open("Punctuation.inc", "w") as f:
        f.write(code)
    print("Punctuation.inc generated successfully!")