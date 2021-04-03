var V86 = require("./libv86.js").V86;

var SCREEN_WIDTH = 80;

function get_line(screen, y) {
	return screen.subarray(y * SCREEN_WIDTH, (y + 1) * SCREEN_WIDTH);
}

function line_to_text(screen, y) {
	return bytearray_to_string(get_line(screen, y));
}

function bytearray_to_string(arr) {
	return String.fromCharCode.apply(String, arr);
}

function screen_to_text(s) {
	var result = [];
	result.push("+==================================== SCREEN ====================================+");

	for(var i = 0; i < 25; i++) {
		var line = line_to_text(s, i);
		result.push("|" + line + "|");
	}

	result.push("+================================================================================+");

	return result.join("\n");
}

var settings = {
	bios: {
		url: "os-test/seabios.bin"
	},
	vga_bios: {
		url: "os-test/vgabios.bin"
	},
	autostart: true,
	memory_size: 128 * 1024 * 1024,
	cdrom: {
		url: "cdrom.iso"
	}
};

var screen = new Uint8Array(SCREEN_WIDTH * 25)

var emulator = new V86(settings);

var trycount = 0;
var boot_enter = true;

setInterval(() => {
	//console.log(screen_to_text(screen));

	if(line_to_text(screen, 4).indexOf("NextOS") != -1 && boot_enter) {
		emulator.keyboard_send_text("\n");
		console.log("Sending enter to boot!");

		boot_enter = false;

		setTimeout(() => {
			emulator.restart();
			trycount++;
			if(trycount > 11) {
				throw new Error("Oh No could not pass test with " + trycount + " try's!");
			}
			console.log("Restarting! try: " + trycount);
			boot_enter = true;
		}, 5000);
	}

	if(line_to_text(screen, 24).startsWith("Running proceses: 1")) {
		console.log("Test Passed!");
		console.log(screen_to_text(screen));
		process.exit(0);
	}
}, 1000);

emulator.add_listener("screen-put-char", function(chr) {
	var y = chr[0];
	var x = chr[1];
	var code = chr[2];
	screen[x + SCREEN_WIDTH * y] = code;
});