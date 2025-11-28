import hw

class Player(ScriptableObject):
	def on_start(self):
		self.scale.x = 2
		self.scale.y = 3
	def on_update(self, ts):
		self.rotation.x += 1 * ts.get_seconds
		self.rotation.y += 3 * ts.get_seconds
		self.rotation.z += 5 * ts.get_seconds