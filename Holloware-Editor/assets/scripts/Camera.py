import hw

class Camera(ScriptableObject):
	def on_start(self):
		pass
	def on_update(self):
		self.rotation.z -= 0.002