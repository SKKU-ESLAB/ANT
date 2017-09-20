{
	"targets": [
		{
			"target_name": "communication-api",
			"sources": [
				"./src/crc_ccit.cc",
				"./src/comm_bt.cc",
				"./src/comm_core.cc",
				"./comm_binding.cc",
				"./NIL_Comm.cc"
			],
			"include_dirs": ["<!(node -e \"require('nan')\")", "./inc"],
			"libraries": ['-lbluetooth', '-fpermissive'],
			"cflags": ['-std=gnu++0x']
		}
	]
}
