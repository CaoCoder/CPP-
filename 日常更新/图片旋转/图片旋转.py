from PIL import Image

# 读取图片
image_path = "rotated_image.jpg"
image = Image.open(image_path)

# 将图片向右旋转45°
rotated_image = image.rotate(15, resample=Image.BICUBIC, expand=True)

# 保存旋转后的图片
output_path = "1.jpg"
rotated_image.save(output_path)


