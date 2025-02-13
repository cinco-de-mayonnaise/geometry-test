# Geometry Test
Playing around with some examples, eventually plan to make some full fleged visual arts demonstrations similar to demo scene cracktros. :P


### Milestones
There are no releases, every major milestone(tag) will be marked as VXX, which should be fully usable/complete and compile without errors. To grab these, go to the relevant tag and download the source code of that commit.

| Milestone(Tag)    | Short Description | API |
| :--------: | :------- | :----- |
| V1 | Small spinning triangle in SDL2 | SDL2 |
| V2 | Spinning triangle but in raylib     | raylib |
|*(ongoing)* | Scrolling Stars (parallax scrolling)  | raylib |

### Compile

Ensure SDL2, SDL2_ttf and raylib are installed. The makefile is not smart enough and compiles everything, even applications that may not depend on all libraries. Then painstakingly type in the following commands and run them.

```
make 
```
(lol)

### Notes
I actually started out with really wanting to learn SDL3, but SDL3 is a pain in the butt to use on Linux for now... I'll have to wait for it to be available on apt repositories...
But then I discovered raylib and realized it's way easier/cooler than SDL, so I'll be using this mu hahaha haha... ha.
