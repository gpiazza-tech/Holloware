from hw_input import key_down
from hw_input import Key
from hw import Entity

class Camera(ScriptableObject):
	def __init__(self):
		self.player = Entity()
	def on_start(self):
		pass
	def on_update(self, ts):
		if self.player.valid:
			self.transform.position.x = self.player.transform.position.x
			self.transform.position.y = self.player.transform.position.y
		else:
			if key_down(Key.LEFT):
				self.rotation.z -= 1.2 * ts.get_seconds
			if key_down(Key.RIGHT):
				self.rotation.z += 1.2 * ts.get_seconds