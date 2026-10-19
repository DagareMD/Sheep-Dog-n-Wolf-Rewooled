# Language packs

A language pack adds a language to Rewooled, or replaces one, without touching the game's files. It's a folder of
plain text files and pictures in your game folder's `Languages/` folder.

## Choosing a language

- **On the language wheel** at the start of the game: turn it and confirm. Every language Rewooled has is on it.
- **In the pause menu**, under **Rewooled Settings**:
  - **Text Language** changes the text. It takes effect when the next level loads.
  - **Voice Language** changes the voices, straight away. **Same as text** uses the voices that go with the text
    language.

Your choices are kept between sessions.

## Installing a pack

1. Put the pack's folder in the `Languages` folder of your game folder (beside `Levels`). Make the folder if it isn't
   there.
2. Start Rewooled. The pack is on the wheel and in Rewooled Settings.

Packs are listed first, sorted by folder name, then the rest. To set their order, start the folder names with numbers
of the same length: `01 Klingon`, `02 Português`. Only the pack's name is shown, so the numbers aren't seen.

A pack with the same name as a language already in the list is used in its place: a pack named `English` is English.
Names are compared without regard to the case of A to Z; an accented letter has to match exactly. Two packs with the
same name: only the first is listed.

## Making a pack

The launcher writes a pack for you from any copy of the game, PC or PlayStation, any release. A pack from a PlayStation
copy has no voices yet (see the notes on the game's versions, below).

1. Open the launcher and click **Language Export**.
2. Pick the game's files: **Select Game Folder** for a game folder, or **Select Game ISO** for a disc image (`.iso`,
   `.bin` or `.cue`, read as it is).
3. Pick the **Language** to start from. The list shows the languages those files have.
4. Give the pack a **name**. It starts as the language's name. The name is also the pack's folder name.
5. Tick **Include voices** to copy that language's voices into the pack, and **Include textures** to get the game's
   pictures with writing on them, ready to redraw.
6. Check **Export to...** (your game folder's `Languages` at first), then click **Export**.

The launcher never writes over an existing folder: give the pack another name, or move the old one out first. If an
export fails, it leaves nothing behind.

Then translate the pack's files (below), and start Rewooled to play it.

## What's in a pack

```
Languages/
  Português/
    language.txt      the pack's name and voices
    text/             all of the game's text
      Lvl-00.txt ... Lvl-17.txt, Intro.txt, Scene.txt, Wheel.txt, Fend.txt, Ending.txt
      Menus.txt       the menus
      Saves.txt       the saving and loading messages
    voices/           the spoken lines (optional)
    font/             the font the text is drawn in
      font.png
      characters.txt
    textures/         the game's pictures with writing on them (optional)
    wheel.png         the pack's picture on the language wheel
```

Everything is the pack's own. Anything it leaves out is left out: a missing line is blank in the game. The one
exception is voices: a pack with no voices of its own speaks with the game's English voices.

## language.txt

```
name = Português
voices = Português
```

- **`name`**: the name shown on the wheel and in Settings. Without it, the folder's name is used.
- **`voices`**: the voices this language starts with: a language's name, from the list. Without it, the pack uses its
  own `voices/` folder. If the pack has no voices of its own, or the language named has none, the game's English voices
  are used.

## Translating the text

Each file is plain text (UTF-8). Every line of the game has a header starting with `@`, and its text under it:

```
@3.0
DINAMITE:
A DINAMITE É MUITO ÚTIL PARA ABRIR PASSAGEM.
UMA VEZ COLOCADA, LEVA O SEU TEMPO PARA EXPLODIR.

@31.0
$10$
$28$Olá... Eu s-s-sou o agricultor Porky.
```

- **Change the text, never the headers.** An exported pack has every header in place.
- The text runs to the next header. Line breaks are kept; blank lines before a header are ignored.
- Keep the game's markers as they are: `$28$` (how long a page stays up), `$C_RED$` (a colour), `$B_JUMP$` (a button
  picture), `$G_NAME$` (a name).
- A line of text that itself starts with `@` is written `@@`.
- `Menus.txt` also holds Rewooled's own menu items (Rewooled Settings and its options, headers `@0x40.0x1` to
  `@0x40.0x9`).
- `Saves.txt` holds the memory card messages. Languages that have none of their own get the English ones, to
  be translated.

## Voices

`voices/` holds WAV files with the game's own clip names, such as `CIN_LVL_17_OB03A.WAV`. An export with **Include
voices** ticked copies them for you. A clip the folder doesn't have isn't played. A pack with a `voices/` folder can be
chosen as a Voice Language in Settings.

## The font

The pack's text is drawn in its `font/`. Every export includes the game's own font, so you only change it to add
letters your language needs.

- **`font.png`** is a grid of 16 × 14 cells, each 16 × 16 pixels (256 × 224 in all). It can also be drawn larger at a
  whole multiple, like 512 × 448, for sharper letters.
- **`characters.txt`** lists the character each cell draws, left to right and top to bottom. A cell holding a button
  picture, or unused, is a space.
- The game draws each letter from one of three cells at random, for a hand-drawn look, so most letters are in the font
  three times. Keep the button pictures in their cells.
- The font has capitals only: a small letter is drawn as its capital. A character the font doesn't have is drawn as
  `?`, and noted in the log.
- Intro's title lettering isn't the font: it's a picture, which you change in `textures/`.

## Textures

With **Include textures** ticked, the export puts every picture with writing on it into `textures/`: signs, "GOAL",
"GAME OVER", title lettering. Each picture is there once, however many levels use it.

- **Redraw the ones you translate.** Keep each one's size, or draw it larger at a whole multiple (2×, 3×, 4×...) for a
  sharper picture, up to 8×. Pictures on the same level page are drawn at one scale, so use the same multiple for all
  of them: 2×, 4× or 8×.
- **Delete the ones you don't change**, or leave them: an unchanged picture changes nothing.
- Don't rename them. A picture's file name says which of the game's pictures it replaces.
- A sign the game draws in several pieces comes as one picture, such as `studio-logo.png`.

## The wheel picture

`wheel.png` is the pack's slice of the language wheel. The export gives you that language's own picture from the
game, or the English one to draw over if the game has none. It's a triangle: its point at the top middle is the
wheel's centre, and its bottom edge is the wheel's rim. Any size works; the game's own are 180 × 124 or 128 × 128. A
pack without one shows a sheep icon on its slice.

## Notes on the game's versions

- Every version has French, English, Spanish, Italian, German and Dutch text. All but the PlayStation PAL version also
  have Brazilian Portuguese.
- An export from a PlayStation disc has no voices yet: the disc keeps them all in one file (`VOICES.XA`), and taking a
  language's voices out of it isn't added to the export yet.
- The Portuguese fan translation ("O Ladrão de Ovelhas") is recognised: its export offers its European Portuguese,
  with its voices and its redrawn pictures.

## For advanced makers

- **Text headers:** a level file's header is `@<list>.<line>` in decimal, with line up to 255. `Menus.txt` uses
  `@0x<group>.0x<index>`; decimal or lower-case hex also work. `Saves.txt` uses the message number, `@0` to `@50`.
- **Texture names:** a picture's file name is its fingerprint: 16 hex digits, an FNV-1a 64-bit hash. It covers the
  picture's pixel format (format & 3, one byte), its width and height (16-bit little-endian each), and its texels as
  the level file stores them (16-bit little-endian, row by row). PlayStation level files are converted to the PC layout
  first. The same picture has the same fingerprint in every level and version that has it unchanged.
- **Texture overrides** (`retexture/`, `retexture-psx/`) don't stop a pack's textures from applying: they're pasted
  over the override.
