from pathlib import Path
import audioop
import wave


ROOT = Path(__file__).resolve().parents[1]
ASSETS = ROOT / "assets"
OUTPUT = ASSETS / "audio_compact"
SOUNDS = (
    ("founded.wav", "founded.wav", 12000),
    ("game.wav", "game.wav", 8000),
    ("stomp.wav", "stomp.wav", 12000),
)


def convert(source: Path, target: Path, rate: int) -> None:
    with wave.open(str(source), "rb") as wav:
        width = wav.getsampwidth()
        channels = wav.getnchannels()
        source_rate = wav.getframerate()
        frames = wav.readframes(wav.getnframes())
    is_stomp = source.name == "stomp.wav"
    if channels == 2:
        frames = audioop.tomono(frames, width, 0.5, 0.5)
    elif channels != 1:
        raise ValueError(f"unsupported channel count: {channels}")
    if is_stomp:
        frames = audioop.mul(frames, width, 5.0)
        source_rate = round(source_rate * 0.60)
    frames, _ = audioop.ratecv(frames, width, 1, source_rate, rate, None)
    frames = audioop.lin2lin(frames, width, 1)
    frames = audioop.bias(frames, 1, 128)
    if is_stomp:
        hit = frames[: rate * 24 // 100]
        frames = hit + bytes([128]) * (rate * 6 // 100) + hit
    with wave.open(str(target), "wb") as wav:
        wav.setnchannels(1)
        wav.setsampwidth(1)
        wav.setframerate(rate)
        wav.writeframes(frames)


def main() -> None:
    OUTPUT.mkdir(parents=True, exist_ok=True)
    for source_name, target_name, rate in SOUNDS:
        source = ASSETS / source_name
        if not source.exists():
            raise FileNotFoundError(source)
        convert(source, OUTPUT / target_name, rate)


if __name__ == "__main__":
    main()
