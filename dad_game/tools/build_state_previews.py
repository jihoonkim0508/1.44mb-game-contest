from pathlib import Path
from shutil import copy2

from PIL import Image, ImageDraw, ImageFont


ROOT = Path(__file__).resolve().parents[2]
GENERATED = Path.home() / ".codex/generated_images/01a01e4d-4779-7c10-a88f-7769490b4557"
OUT = ROOT / "output/imagegen/dad-game-states-v3"
SOURCES = OUT / "sources"
SPRITES = OUT / "sprites"
STATES = OUT / "states"

SOURCE_FILES = {
    "background": "exec-16ae96bd-b8eb-4f7f-afe5-0cbd7bd62065.png",
    "furniture": "exec-d82b69d0-4cd3-41e8-a049-db9e5cec2bbf.png",
    "player": "exec-e14bcf69-adbe-400a-b3b2-625d26ed161d.png",
    "door_father": "exec-a2045b85-20fb-4e77-bbbe-16200f565e27.png",
}


def hard_crop(image: Image.Image, box: tuple[int, int, int, int]) -> Image.Image:
    crop = image.crop(box).convert("RGBA")
    alpha = crop.getchannel("A").point(lambda value: 255 if value >= 128 else 0)
    crop.putalpha(alpha)
    bounds = alpha.getbbox()
    if not bounds:
        raise ValueError(f"No opaque pixels in crop {box}")
    return crop.crop(bounds)


def fit(sprite: Image.Image, size: tuple[int, int]) -> Image.Image:
    return sprite.resize(size, Image.Resampling.NEAREST)


def paste(canvas: Image.Image, sprite: Image.Image, xy: tuple[int, int], size: tuple[int, int]) -> None:
    layer = fit(sprite, size)
    canvas.alpha_composite(layer, xy)


def font(size: int) -> ImageFont.FreeTypeFont | ImageFont.ImageFont:
    path = Path("C:/Windows/Fonts/malgunbd.ttf")
    return ImageFont.truetype(path, size) if path.exists() else ImageFont.load_default()


def main() -> None:
    for folder in (SOURCES, SPRITES, STATES):
        folder.mkdir(parents=True, exist_ok=True)

    local_sources: dict[str, Path] = {}
    for key, filename in SOURCE_FILES.items():
        source = GENERATED / filename
        target = SOURCES / f"{key}.png"
        copy2(source, target)
        local_sources[key] = target

    furniture = Image.open(local_sources["furniture"]).convert("RGBA")
    player = Image.open(local_sources["player"]).convert("RGBA")
    door_father = Image.open(local_sources["door_father"]).convert("RGBA")

    sprites = {
        "bed": hard_crop(furniture, (0, 0, 800, 545)),
        "desk": hard_crop(furniture, (790, 0, 1536, 545)),
        "monitor_off": hard_crop(furniture, (0, 520, 520, 845)),
        "monitor_on": hard_crop(furniture, (520, 520, 1030, 845)),
        "monitor_playing": hard_crop(furniture, (1030, 520, 1536, 845)),
        "keyboard": hard_crop(furniture, (350, 820, 930, 1024)),
        "mouse": hard_crop(furniture, (930, 820, 1160, 1024)),
        "player_sitting": hard_crop(player, (0, 120, 650, 900)),
        "player_sleeping": hard_crop(player, (650, 120, 1536, 900)),
        "door_closed": hard_crop(door_father, (0, 50, 385, 930)),
        "door_open": hard_crop(door_father, (775, 50, 1190, 930)),
        "father": hard_crop(door_father, (1180, 50, 1536, 960)),
    }
    for name, sprite in sprites.items():
        sprite.save(SPRITES / f"{name}.png")

    background_source = Image.open(local_sources["background"]).convert("RGB")
    background_360 = background_source.resize((360, 360), Image.Resampling.LANCZOS)
    background_360 = background_360.quantize(colors=32, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE).convert("RGB")
    background = background_360.resize((720, 720), Image.Resampling.NEAREST).convert("RGBA")
    background_360.save(OUT / "background-360.png")

    player_monitor_states = [
        ("sitting-monitor-on", "player_sitting", "monitor_on"),
        ("sitting-playing", "player_sitting", "monitor_playing"),
        ("sitting-monitor-off", "player_sitting", "monitor_off"),
        ("sleeping-monitor-on", "player_sleeping", "monitor_on"),
        ("sleeping-monitor-off", "player_sleeping", "monitor_off"),
    ]
    arrival_states = [
        ("door-closed", "door_closed", None),
        ("father-entered", "door_open", "father"),
    ]

    raw_states: list[tuple[str, Image.Image]] = []
    for arrival_name, door_name, father_name in arrival_states:
        for pose_name, player_name, monitor_name in player_monitor_states:
            canvas = background.copy()
            if door_name == "door_closed":
                paste(canvas, sprites[door_name], (540, 20), (158, 250))
            else:
                paste(canvas, sprites[door_name], (554, 20), (144, 250))
            if father_name:
                paste(canvas, sprites[father_name], (575, 42), (105, 230))

            paste(canvas, sprites["bed"], (80, 520), (560, 160))
            paste(canvas, sprites["desk"], (180, 270), (359, 280))

            monitor_positions = {
                "monitor_on": ((230, 85), (260, 208)),
                "monitor_playing": ((225, 90), (270, 208)),
                "monitor_off": ((227, 90), (267, 208)),
            }
            monitor_xy, monitor_size = monitor_positions[monitor_name]
            paste(canvas, sprites[monitor_name], monitor_xy, monitor_size)
            paste(canvas, sprites["keyboard"], (215, 340), (300, 118))
            paste(canvas, sprites["mouse"], (485, 385), (35, 47))

            if player_name == "player_sleeping":
                paste(canvas, sprites[player_name], (173, 545), (374, 160))
            else:
                paste(canvas, sprites[player_name], (255, 390), (215, 320))

            name = f"{arrival_name}__{pose_name}"
            raw_states.append((name, canvas.convert("RGB")))

    palette_sample = Image.new("RGB", (720, len(raw_states) * 90))
    for index, (_, scene) in enumerate(raw_states):
        palette_sample.paste(scene.resize((180, 180), Image.Resampling.NEAREST), (0, index * 90))
        palette_sample.paste(scene.resize((180, 180), Image.Resampling.NEAREST), (180, index * 90))
        palette_sample.paste(scene.resize((180, 180), Image.Resampling.NEAREST), (360, index * 90))
        palette_sample.paste(scene.resize((180, 180), Image.Resampling.NEAREST), (540, index * 90))
    master = palette_sample.quantize(colors=48, method=Image.Quantize.MEDIANCUT, dither=Image.Dither.NONE)

    saved: list[tuple[str, Image.Image]] = []
    for index, (name, scene) in enumerate(raw_states, 1):
        final = scene.quantize(palette=master, dither=Image.Dither.NONE).convert("RGB")
        filename = f"{index:02d}-{name}.png"
        final.save(STATES / filename, optimize=True)
        saved.append((filename, final))

    thumb_size = 288
    label_height = 48
    sheet = Image.new("RGB", (thumb_size * 5, (thumb_size + label_height) * 2), (20, 24, 28))
    draw = ImageDraw.Draw(sheet)
    label_font = font(16)
    for index, (filename, scene) in enumerate(saved):
        x = (index % 5) * thumb_size
        y = (index // 5) * (thumb_size + label_height)
        sheet.paste(scene.resize((thumb_size, thumb_size), Image.Resampling.NEAREST), (x, y))
        draw.text((x + 8, y + thumb_size + 5), filename.removesuffix(".png"), font=label_font, fill=(245, 240, 220))
    sheet.save(OUT / "all-10-states-contact-sheet.png", optimize=True)

    (OUT / "STATE_INDEX.txt").write_text(
        "Reachable visual gameplay states: 5 player/monitor states x 2 arrival states = 10\n\n"
        + "\n".join(filename for filename, _ in saved)
        + "\n",
        encoding="utf-8",
    )


if __name__ == "__main__":
    main()
