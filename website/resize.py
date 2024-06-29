import os
from PIL import Image

# 定义路径
input_path = r"C:\Users\laish\surfaceStudio\Work\Dev\djjxad\images\work"
output_path = r"C:\Users\laish\surfaceStudio\Work\Dev\djjxad\images\work\resized"

# 创建输出目录（如果不存在）
if not os.path.exists(output_path):
    os.makedirs(output_path)

# 获取所有图片文件
files = [f for f in os.listdir(input_path) if os.path.isfile(os.path.join(input_path, f)) and f.lower().endswith(('.png', '.jpg', '.jpeg'))]

# 调整大小并重命名
for i, file in enumerate(files, start=1):
    img = Image.open(os.path.join(input_path, file))
    img = img.resize((1024, 635), Image.LANCZOS)
    if img.mode == 'RGBA':
        img = img.convert('RGB')
    img.save(os.path.join(output_path, f"{i}.jpg"), "JPEG")

print("所有图片已调整大小并重命名。")
