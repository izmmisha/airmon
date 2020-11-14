CWD := $(shell pwd)
DOCKER_EXE := docker

DOCKERFILE := docker/Dockerfile

PROJECT_DIR =
ENABLE_TTY =
MAKE_DOCKER = $(DOCKER_EXE) build - < $(DOCKERFILE) | tee /dev/stderr | grep "Successfully built" | cut -d ' ' -f 3
RUN = $(DOCKER_EXE) run \
  -v $(CWD):/project $(PROJECT_DIR) \
  --user $(shell id -u):$(shell id -g) \
  --rm \
  -i $(ENABLE_TTY) `$(MAKE_DOCKER)`

build: PROJECT_DIR=-w /project/src
build:
	@$(RUN) idf.py build

shell: ENABLE_TTY=-t
shell:
	@$(RUN) bash

docker:
	@$(MAKE_DOCKER)

