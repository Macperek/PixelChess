import re
import sys

def remove_comments(text):
    # Wyrażenie regularne dopasowujące komentarze // oraz /* */
    pattern = r'(//.*?$|/\*.*?\*/)'
    # Używamy flagi DOTALL dla komentarzy wieloliniowych i MULTILINE dla liniowych
    return re.sub(pattern, '', text, flags=re.DOTALL | re.MULTILINE)

def process_file(filename):
    try:
        with open(filename, 'r', encoding='utf-8') as f:
            content = f.read()
        
        clean_content = remove_comments(content)
        
        with open(filename, 'w', encoding='utf-8') as f:
            f.write(clean_content)
        print(f"Oczyszczono: {filename}")
    except Exception as e:
        print(f"Błąd przy pliku {filename}: {e}")

if __name__ == "__main__":
    # Przyjmuje listę plików jako argumenty
    for arg in sys.argv[1:]:
        process_file(arg)