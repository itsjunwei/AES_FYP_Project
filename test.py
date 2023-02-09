with open('./data/plaintext.txt') as f:
    seen = set()
    for line in f:
        line_lower = line.lower().strip()
        if line_lower in seen:
            print(line)
        else:
            seen.add(line_lower)