window.flutterWebRendererReady = function () {
	let splash = document.getElementById('splash-screen');
	if (splash !== undefined) {
		splash.style.opacity = '0';
		setTimeout(() => (splash.style.display = 'none'), 500);
	}
};
