ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

.DEFAULT_GOAL := help

help: ## Display available commands in Makefile
	@grep -hE '^[a-zA-Z_0-9-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

build-docker-image:  ## Build the docker image
	docker build . -t ubuntuwithopencv:1 --build-arg uid=$(shell id -u) --build-arg gid=$(shell id -g) --build-arg user=dockeruser --build-arg group=dockergroup

shell:  ## Start a shell attached to the docker image created with build-docker-image
	docker build . -t ubuntuwithopencv:1 --build-arg uid=$(shell id -u) --build-arg gid=$(shell id -g) --build-arg user=dockeruser --build-arg group=dockergroup
	xhost +local:docker
	docker run --rm --device /dev/video0:/dev/video0 --user $(shell id -u):$(shell id -g) -v $(shell pwd):/workdir -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=$(DISPLAY) -w /workdir -it ubuntuwithopencv:1 bash

all: 0-helloworld 1-read-and-show-image 2-resize ## Compilation line. Execute in a shell attached to ubuntuwithopencv:1

0-helloworld: .phony ## Compilation line. Execute in a shell attached to ubuntuwithopencv:1
	g++ $@.cc `pkg-config --cflags --libs opencv4` -o $@

1-read-and-show-image: .phony ## Compilation line. Execute in a shell attached to ubuntuwithopencv:1
	g++ $@.cc `pkg-config --cflags --libs opencv4` -o $@

2-resize: .phony ## Compilation line. Execute in a shell attached to ubuntuwithopencv:1
	g++ $@.cc `pkg-config --cflags --libs opencv4` -o $@

clean:
	rm -f *.swp 0-helloworld 1-read-and-show-image 2-resize

.phony:

