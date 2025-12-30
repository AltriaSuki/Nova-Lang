# Benchmarks

**Status:** Lexer benchmark implemented (`nova-bench`).

## Purpose
This directory is reserved for benchmarks that measure compiler performance.

## Benchmark Categories
- [ ] Lexer throughput
- [ ] Parser throughput
- [ ] Compilation time
- [ ] Generated code performance
- [ ] Memory usage

## Running
```bash
cd build
./bin/nova-bench
```

Examples:
```bash
./bin/nova-bench --bytes 1000000 --repeat 200
./bin/nova-bench --file ../examples/hello.nova --repeat 1000
```

## Tracking
Benchmark tracking infrastructure is not yet provided.
