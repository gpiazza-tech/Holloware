import hw

class Camera(ScriptableObject):
	def on_start(self):
		hw.log("Camera Started!")
	def on_update(self):
		hw.log("Camera Updating!")