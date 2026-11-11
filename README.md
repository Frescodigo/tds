# A simple top-down shooter
Made using the [raylib quickstart repo](https://github.com/raylib-extras/raylib-quickstart).

## Goals for the project
1. Make a project using the [raylib library](https://www.raylib.com/) (link to [github](https://github.com/raysan5/raylibhttps://github.com/raysan5/raylib))
2. Finish a project with limited scope in a short timeframe (~1 day)

## Scope for the project
- [X] player with custom sprite
- [X] player can move with WASD
- [X] player can shoot with LMB
- [X] bullets are removed when they leave the screen
- [X] enemies that spawn in random locations
- [X] enemies have custom sprite
- [X] enemies move towards player
- [X] enemies die when shot
- [X] score
- [X] death
- [X] sfx
- [X] menu

### Stretch goals
- [ ] multiple enemy types
- [ ] multiple "guns" (burst?)
- [ ] health
- [ ] project structure


## Notes on implementation
**No AI was used in the process. No code or guidance.**

I did not care about architecture very much.
I was totally okay with writing spaghetti code just to get the features
checked off.
In the future, I want to ensure correctness and maintanability of code.
For now, I just want to finish writing something.

For future, intrusive lists for different entity types.
Like, there can be an array `enemies` which containes indexes into the
main `entities` array. Not really sure how to do updates of different
entity types otherwise. A switch statement seems really bad for performance.

Using a switchback array had weird behavior when deleting

