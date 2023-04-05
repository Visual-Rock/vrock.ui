# Utils

- [List\<T\>](./include/vrock/utils/List.md)

## meson

create a file with the name `vrockui.wrap`
and fill it with the following content

```text
[wrap-git]
url=https://github.com/Visual-Rock/vrock.ui
revision=head
depth=1

[provide]
vrockui=vrockui_dep
```
