import hw

class Player(ScriptableObject):
	def on_start(self):
		self.scale.x = 2
		self.scale.y = 3
	def on_update(self):
		self.rotation.x += 0.001
		self.rotation.y += 0.003
		self.rotation.z += 0.005