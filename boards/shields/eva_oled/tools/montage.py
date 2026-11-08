# Montage of the host-rendered PBM frames, styled like the OLED (blue).
import sys, glob, os
from PIL import Image, ImageDraw
d = sys.argv[1]; out = sys.argv[2]
names = ['L_base', 'L_lower_caps', 'L_game', 'L_gnum_usb', 'L_emergency', 'R_nerv_idle', 'R_nerv_ripples', 'R_warn_bat', 'R_warn_link']
S = 4; pad = 14
W, H = 32 * S, 128 * S
img = Image.new('RGB', (len(names) * (W + pad) + pad, H + 2 * pad + 14), (14, 16, 18))
dr = ImageDraw.Draw(img)
for i, n in enumerate(names):
    lines = open(os.path.join(d, n + '.pbm')).read().split('\n')[2:]
    ox = pad + i * (W + pad)
    dr.rectangle([ox - 2, pad - 2, ox + W + 1, pad + H + 1], fill=(3, 4, 5))
    for y, row in enumerate(lines[:128]):
        for x, c in enumerate(row):
            if c == '1':
                dr.rectangle([ox + x * S, pad + y * S, ox + x * S + S - 2, pad + y * S + S - 2], fill=(79, 195, 255))
    dr.text((ox, pad + H + 4), n, fill=(200, 200, 200))
img.save(out)
print(out)
