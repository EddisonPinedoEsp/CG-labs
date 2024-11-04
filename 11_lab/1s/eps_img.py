from PIL import Image
import sys

def convert_eps_to_jpg(eps_path, jpg_path):
    with Image.open(eps_path) as img:
        img.load(scale=10)  # Load the image with a higher resolution
        img = img.convert("RGB")  # Convert to RGB mode
        img.save(jpg_path, 'JPEG')

if len(sys.argv) != 3:
    print("Usage: python eps_img.py <input_eps_path> <output_jpg_path>")
    sys.exit(1)

eps_path = sys.argv[1]
jpg_path = sys.argv[2]
convert_eps_to_jpg(eps_path, jpg_path)
print(f"Converted {eps_path} to {jpg_path}")