import re
from pathlib import Path

from PIL import Image, ImageChops, ImageDraw, ImageEnhance


ROOT = Path(__file__).resolve().parents[1]
ASSETS = ROOT / "assets"
CONCEPTS = ASSETS / "concepts"
FINAL = ASSETS / "final"
STATES = FINAL / "states"

ROOM_SOURCE = CONCEPTS / "scene-room-canonical-player-free-v1.png"
BASE_SOURCE = CONCEPTS / "dad-game-base-reference-layout-v5.png"
CHARACTERS_SOURCE = CONCEPTS / "layers/character-poses-consistent-v2.png"
FATHER_SOURCE = CONCEPTS / "layers/layer-door-open-father-v2.png"
FONT_ATLAS_SOURCE = ASSETS / "font_atlas.png"
FONT_HEADER_SOURCE = ROOT / "font_atlas_data.h"

MAGENTA = (255, 0, 255)
SHEET_RECTS = {
    "sitting": (0, 0, 185, 190),
    "sleeping": (190, 0, 220, 100),
    "father": (190, 105, 116, 164),
    "screen": (310, 105, 164, 95),
}
SCENE_RECTS = {
    "sitting": (70, 155, 185, 190),
    "sleeping": (90, 250, 220, 100),
    "father": (242, 9, 116, 164),
    "screen": (84, 77, 164, 95),
}


def crop_alpha(image: Image.Image, box: tuple[int, int, int, int] | None = None) -> Image.Image:
    source = image.crop(box) if box else image
    alpha = source.getchannel("A").point(lambda value: 255 if value >= 128 else 0)
    bounds = alpha.getbbox()
    if not bounds:
        raise ValueError("transparent sprite")
    sprite = source.crop(bounds).convert("RGBA")
    sprite.putalpha(alpha.crop(bounds))
    return sprite


def fit(sprite: Image.Image, size: tuple[int, int], colors: int = 32) -> Image.Image:
    sprite = sprite.resize(size, Image.Resampling.LANCZOS)
    alpha = sprite.getchannel("A").point(lambda value: 255 if value >= 128 else 0)
    rgb = sprite.convert("RGB").quantize(colors=colors, dither=Image.Dither.NONE).convert("RGB")
    rgb.putalpha(alpha)
    return rgb


def paste(canvas: Image.Image, sprite: Image.Image, rect: tuple[int, int, int, int]) -> None:
    x, y, width, height = rect
    fitted = fit(sprite, (width, height))
    canvas.paste(fitted, (x, y), fitted.getchannel("A"))


def load_font_data() -> tuple[str, list[int]]:
    header = FONT_HEADER_SOURCE.read_text(encoding="utf-8")
    chars_match = re.search(r'font_chars\[\]=L"([^"]+)"', header)
    widths_match = re.search(r"font_widths\[\]=\{([^}]+)\}", header)
    if not chars_match or not widths_match:
        raise ValueError("invalid generated font header")
    chars = chars_match.group(1)
    widths = [int(value) for value in widths_match.group(1).split(",")]
    if len(chars) != len(widths):
        raise ValueError("font glyph and width counts differ")
    return chars, widths


def atlas_text_layer(
    atlas: Image.Image,
    chars: str,
    widths: list[int],
    label: str,
    bank: int,
    height: int,
) -> Image.Image:
    indices = [chars.find(char) if char != " " else -2 for char in label]
    fallback = chars.index("?")
    tracking = 1
    source_width = sum(6 if index == -2 else widths[index if index >= 0 else fallback] for index in indices)
    source_width += tracking * max(0, len(indices) - 1)
    layer = Image.new("RGBA", (source_width, 20), (0, 0, 0, 0))
    x = 0
    for position, index in enumerate(indices):
        if index == -2:
            x += 6 + (tracking if position + 1 < len(indices) else 0)
            continue
        if index < 0:
            index = fallback
        width = widths[index]
        source_x = (index % 32) * 16
        source_y = bank * 60 + (index // 32) * 20
        tile_rgb = atlas.crop((source_x, source_y, source_x + width, source_y + 20)).convert("RGB")
        mask = ImageChops.difference(tile_rgb, Image.new("RGB", tile_rgb.size, MAGENTA)).convert("L")
        mask = mask.point(lambda value: 255 if value else 0)
        tile = tile_rgb.convert("RGBA")
        tile.putalpha(mask)
        layer.alpha_composite(tile, (x, 0))
        x += width + (tracking if position + 1 < len(indices) else 0)
    if height != 20:
        layer = layer.resize((round(layer.width * height / 20), height), Image.Resampling.NEAREST)
    return layer


def draw_atlas_text(
    scene: Image.Image,
    atlas: Image.Image,
    chars: str,
    widths: list[int],
    label: str,
    center: tuple[int, int],
    height: int,
    bank: int,
) -> None:
    body = atlas_text_layer(atlas, chars, widths, label, bank, height)
    outline = atlas_text_layer(atlas, chars, widths, label, 3, height)
    x = round(center[0] - body.width / 2)
    y = round(center[1] - body.height / 2)
    distance = 2 if height >= 30 else 1
    for dx, dy in ((-distance, -distance), (0, -distance), (distance, -distance), (-distance, 0), (distance, 0), (-distance, distance), (0, distance), (distance, distance)):
        scene.alpha_composite(outline, (x + dx, y + dy))
    scene.alpha_composite(body, (x, y))


def make_effects(
    scene: Image.Image,
    playing: bool,
    caught: bool,
    font_atlas: Image.Image,
    font_chars: str,
    font_widths: list[int],
) -> None:
    draw = ImageDraw.Draw(scene)
    if playing:
        orange = (255, 105, 18)
        yellow = (255, 225, 92)
        draw.rectangle((79, 72, 252, 74), fill=orange)
        draw.rectangle((79, 175, 252, 177), fill=orange)
        draw.rectangle((79, 72, 81, 177), fill=orange)
        draw.rectangle((250, 72, 252, 177), fill=orange)
        draw.rectangle((211, 108, 229, 115), fill=orange)
        draw.rectangle((217, 102, 223, 121), fill=yellow)
        draw.rectangle((49, 202, 65, 208), fill=orange)
        draw.rectangle((56, 209, 72, 215), fill=yellow)
        draw.rectangle((239, 199, 255, 205), fill=orange)
        draw.rectangle((232, 206, 248, 212), fill=yellow)
        popups = (((38, 91), "+3"), ((286, 102), "+10"), ((47, 169), "+2"), ((298, 157), "+5"), ((126, 62), "+1"), ((215, 187), "+2"))
        for index, (center, label) in enumerate(popups):
            draw_atlas_text(scene, font_atlas, font_chars, font_widths, label, center, 20, 0 if index & 1 else 1)
    if caught:
        red = (193, 35, 35)
        draw.rectangle((0, 0, 359, 7), fill=red)
        draw.rectangle((0, 352, 359, 359), fill=red)
        draw.rectangle((0, 0, 7, 359), fill=red)
        draw.rectangle((352, 0, 359, 359), fill=red)
        draw.rectangle((84, 103, 248, 148), fill=(90, 20, 20))
        draw_atlas_text(scene, font_atlas, font_chars, font_widths, "들켰다!", (166, 125), 30, 0)


def apply_sleep_lighting(scene: Image.Image, monitor_on: bool, screen: Image.Image) -> Image.Image:
    scene = Image.alpha_composite(scene, Image.new("RGBA", scene.size, (3, 7, 18, 175)))
    if monitor_on:
        paste(scene, screen, (84, 77, 164, 95))
    return scene


def main() -> None:
    STATES.mkdir(parents=True, exist_ok=True)

    room_source = Image.open(ROOM_SOURCE).convert("RGB")
    room = room_source.resize((360, 360), Image.Resampling.LANCZOS)
    room = room.quantize(colors=64, dither=Image.Dither.NONE).convert("RGB")
    room.save(ASSETS / "room_background.bmp")

    characters = Image.open(CHARACTERS_SOURCE).convert("RGBA")
    sitting = crop_alpha(characters, (0, 0, 780, characters.height))
    sleeping = crop_alpha(characters, (780, 0, characters.width, characters.height))
    assert abs(sitting.width / sitting.height - SHEET_RECTS["sitting"][2] / SHEET_RECTS["sitting"][3]) < 0.01
    assert abs(sleeping.width / sleeping.height - SHEET_RECTS["sleeping"][2] / SHEET_RECTS["sleeping"][3]) < 0.01
    father = crop_alpha(Image.open(FATHER_SOURCE).convert("RGBA"))

    base = Image.open(BASE_SOURCE).convert("RGB")
    screen = base.crop((292, 268, 866, 602)).convert("RGB")
    screen = ImageEnhance.Brightness(screen).enhance(1.55)
    screen = ImageEnhance.Color(screen).enhance(1.30)
    screen = ImageEnhance.Contrast(screen).enhance(1.12).convert("RGBA")
    screen.putalpha(255)
    sprites = {"sitting": sitting, "sleeping": sleeping, "father": father, "screen": screen}
    father_behind_monitor = fit(father, (116, 164)).crop((18, 0, 116, 164))

    sheet = Image.new("RGB", (512, 512), MAGENTA)
    for name, sprite in sprites.items():
        x, y, width, height = SHEET_RECTS[name]
        fitted = fit(sprite, (width, height))
        sheet.paste(fitted.convert("RGB"), (x, y), fitted.getchannel("A"))
    font_atlas = Image.open(FONT_ATLAS_SOURCE).convert("RGB")
    font_chars, font_widths = load_font_data()
    if font_atlas.size != (512, 240):
        raise ValueError("font atlas must be exactly 512x240")
    sheet.paste(font_atlas, (0, 272))
    sheet.save(ASSETS / "sprite_sheet.bmp")

    player_states = [
        ("sitting-monitor-on", "sitting", True, False),
        ("sitting-playing", "sitting", True, True),
        ("sitting-monitor-off", "sitting", False, False),
        ("sleeping-monitor-on", "sleeping", True, False),
        ("sleeping-monitor-off", "sleeping", False, False),
    ]
    saved: list[Image.Image] = []
    index_lines = []
    for player_name, player_sprite, monitor_on, playing in player_states:
        for door_name, father_present in (("door-closed", False), ("father-entered", True)):
            scene = room.convert("RGBA")
            if father_present:
                scene.alpha_composite(father_behind_monitor, (260, 9))
            if monitor_on:
                paste(scene, screen, SCENE_RECTS["screen"])
            paste(scene, sprites[player_sprite], SCENE_RECTS[player_sprite])
            if player_sprite == "sleeping":
                scene = apply_sleep_lighting(scene, monitor_on, screen)
            caught = father_present and player_name != "sleeping-monitor-off"
            make_effects(scene, playing and not caught, caught, font_atlas, font_chars, font_widths)
            filename = f"{len(saved) + 1:02d}-{door_name}__{player_name}.png"
            preview = scene.convert("RGB").resize((720, 720), Image.Resampling.NEAREST)
            preview.save(STATES / filename, optimize=True)
            saved.append(preview)
            index_lines.append(f"{filename} | caught={int(caught)} | safe={int(father_present and not caught)}")

    sheet_preview = Image.new("RGB", (720 * 5, 720 * 2), (20, 24, 28))
    for index, preview in enumerate(saved):
        sheet_preview.paste(preview, ((index % 5) * 720, (index // 5) * 720))
    sheet_preview.resize((1800, 720), Image.Resampling.NEAREST).save(FINAL / "all-10-states-contact-sheet.png", optimize=True)
    (FINAL / "STATE_INDEX.txt").write_text("\n".join(index_lines) + "\n", encoding="utf-8")

    assert Image.open(ASSETS / "room_background.bmp").size == (360, 360)
    assert Image.open(ASSETS / "sprite_sheet.bmp").size == (512, 512)
    assert len(list(STATES.glob("*.png"))) == 10


if __name__ == "__main__":
    main()
