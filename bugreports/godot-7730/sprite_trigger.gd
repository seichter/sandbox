extends AnimatedSprite

# just output some debug info
func _cb_framechange():
	print(self.get_animation()," ",self.get_frame())

# hook up the signal to the callback on start
func _ready():
	set_process(true)
	# two options
	self.connect("frame_changed",self,"_cb_framechange")
	# get_node(".").connect("frame_changed",self,"_cb_framechange")

func _process(delta):
#	just checking if the process is already connected - and yes it is 
#	if (self.is_connected("frame_changed",self,"_cb_framechange")):
#		print("already connected")
	pass
	