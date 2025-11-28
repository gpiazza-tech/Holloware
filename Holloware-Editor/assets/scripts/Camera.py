import hw

class Camera(ScriptableObject):
	def on_start(self):
		pass
	def on_update(self, ts):
		self.rotation.z += 1.2 * ts.get_seconds