# Changelog

## 1.3.4

- Added changelog

### compiler-launcher

- Added `-h/-help/help` flag to display help in CLI
- Fixed compiler execution time measurement for measurements without time-trace

### Example project

- Added new presets, the available ones are now:
  - `release-gcc`
  - `release-clang`
  - `release-clang-tt`
  - `debug-gcc`
  - `debug-clang`
  - `debug-clang-tt`

### CI/CD

- Now compiling all presets
- Results are uploaded as artifacts
