class Flashcard extends HTMLElement {
	constructor() {
		super();
		const shadow = this.attachShadow({mode: 'open'});
		const div = document.createElement('div');
		const template = document.getElementById('flashcard-template').content;
		const shadowRoot = this.attachShadow({mode: 'open'}).appendChild(template.cloneNode(true));
	}

	render() {
		this.shadowRoot.querySelector("ul[name='choices']");
	}

	get prompt() {
		return this.getAttribute('prompt') || '';
	}

	get choices() {
		return JSON.parse(this.getAttribute('choices') || '[]');
	}

	connectedCallback() {
	}

	disconnectedCallback() {
	}

	adoptedCallback() {
	}

	attributeChangedCallback(name, oldValue, newValue) {
		if (oldValue !== newValue) {
			this.render();
		}
	}
}

customElements.define('revoco-flashcard', Flashcard);
