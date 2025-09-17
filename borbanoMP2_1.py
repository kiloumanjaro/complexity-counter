import sys

KEYWORDS = {
    "int", "char"
}

class Context:
    def __init__(self, type="STATEMENTS", heading_printed=False):
        self.type = type
        self.heading_printed = heading_printed

def trim(s):
    return s.strip()

def clean_line(line):
    cleaned = trim(line)
    if cleaned.endswith(';'):
        cleaned = cleaned[:-1]
    tokens = cleaned.split()
    out = ' '.join(t for t in tokens if t not in KEYWORDS)
    return trim(out)

def tokenize(raw):
    tokens = []
    for line in raw:
        s = trim(line)
        if not s:
            continue
        temp = ""
        paren_depth = 0
        for c in s:
            if c == '(':
                paren_depth += 1
                temp += c
            elif c == ')':
                paren_depth -= 1 if paren_depth > 0 else 0
                temp += c
            elif c in '{}':
                if temp.strip():
                    tokens.append(temp.strip())
                    temp = ""
                tokens.append(c)
            elif c == ';' and paren_depth == 0:
                if temp.strip():
                    tokens.append(temp.strip())
                temp = ""
            else:
                temp += c
        if temp.strip():
            tokens.append(temp.strip())

    final_tokens = []
    for t in tokens:
        tt = t.strip()
        if tt.startswith(("if", "for", "else if")):
            pos = tt.find(')')
            if pos != -1 and pos + 1 < len(tt):
                head = tt[:pos+1].strip()
                rest = tt[pos+1:].strip()
                if head: final_tokens.append(head)
                if rest: final_tokens.append(rest)
            else:
                final_tokens.append(tt)
        else:
            final_tokens.append(tt)
    return final_tokens

def starts_with(s, prefix):
    return s.startswith(prefix)

def parse(tokens):
    output = []
    ctx = [Context("STATEMENTS")]
    top_statements_added = False

    def ensure_top_statements_heading():
        nonlocal top_statements_added
        if not top_statements_added:
            output.append("statements:")
            top_statements_added = True

    i = 0
    while i < len(tokens):
        tok = tokens[i].strip()
        if not tok:
            i += 1
            continue

        first_token_is_if_for = (len(output) == 0) and (starts_with(tok, "if") or starts_with(tok, "for"))

        if starts_with(tok, "if") or starts_with(tok, "else if"):
            lp = tok.find('(')
            rp = tok.find(')', lp if lp != -1 else 0)
            condition = tok[lp+1:rp].strip() if lp != -1 and rp != -1 else ""
            output.append("if:")
            output.append(f"condition: {clean_line(condition)}")

            if i+1 < len(tokens) and tokens[i+1].strip() == "{":
                ctx.append(Context("IF"))
                i += 2
            elif i+1 < len(tokens):
                stmt = tokens[i+1].strip()
                if stmt and stmt not in '{}':
                    output.append("if statements:")
                    output.append(clean_line(stmt))
                    i += 2
                    top_statements_added = False
                else:
                    i += 1

            else:
                i += 1
            continue

        if starts_with(tok, "for"):
            lp = tok.find('(')
            rp = tok.find(')', lp if lp != -1 else 0)
            params = tok[lp+1:rp] if lp != -1 and rp != -1 else ""
            output.append("for:")
            parts = [p.strip() for p in params.split(';')]
            if len(parts) >= 1: output.append("initializer: " + clean_line(parts[0]))
            if len(parts) >= 2: output.append("condition: " + clean_line(parts[1]))
            if len(parts) >= 3: output.append("update: " + clean_line(parts[2]))
        
            output.append("for statements:")
        
            if i+1 < len(tokens) and tokens[i+1].strip() == "{":
                ctx.append(Context("FOR", heading_printed=True))
                i += 2
            elif i+1 < len(tokens):
                stmt = tokens[i+1].strip()
                if stmt and stmt not in '{}':
                    output.append(clean_line(stmt))
                    i += 2
                else:
                    i += 1
            else:
                i += 1
            continue


        if tok == "else":
            if i+1 < len(tokens) and tokens[i+1].strip() == "{":
                ctx.append(Context("ELSE"))
                i += 2
            elif i+1 < len(tokens):
                stmt = tokens[i+1].strip()
                if stmt and stmt not in '{}':
                    output.append("else statements:")
                    output.append(clean_line(stmt))
                    i += 2
                else:
                    i += 1
            else:
                i += 1
            continue

        if tok == "{":
            ctx.append(Context("BLOCK"))
            i += 1
            continue

        if tok == "}":
            if len(ctx) > 1:
                popped = ctx.pop()
                if popped.type == "IF" and ctx[-1].type == "FOR":
                    output.append("for statements continued:")
            i += 1
            continue

        top = ctx[-1]

        def handle_statement(st):
            cleaned = clean_line(st)
            if not cleaned:
                return

            symbols = "=+-/*%<>"
            if not any(sym in cleaned for sym in symbols):
                return

            if top.type == "IF":
                if not top.heading_printed:
                    output.append("if statements:")
                    top.heading_printed = True
                output.append(cleaned)
            elif top.type == "FOR":
                if not top.heading_printed:
                    output.append("for statements:")
                    top.heading_printed = True
                output.append(cleaned)
            elif top.type == "ELSE":
                if not top.heading_printed:
                    output.append("else statements:")
                    top.heading_printed = True
                output.append(cleaned)
            else:
                ensure_top_statements_heading()
                output.append(cleaned)

        depth = 0
        temp = ""
        for c in tok:
            if c == '(':
                depth += 1
                temp += c
            elif c == ')':
                depth -= 1
                temp += c
            elif c == ',' and depth == 0:
                handle_statement(temp.strip())
                temp = ""
            else:
                temp += c
        if temp.strip():
            handle_statement(temp.strip())

        i += 1

    return output

def main():
    lines = int(sys.stdin.readline())
    raw = [sys.stdin.readline().rstrip("\n") for _ in range(lines)]
    tokens = tokenize(raw)
    output = parse(tokens)
    for s in output:
        print(s)

if __name__ == "__main__":
    main()