all: .venv

.venv:
	python -m venv --system-site-packages .venv
	source .venv/bin/activate; pip install -r requirements.txt