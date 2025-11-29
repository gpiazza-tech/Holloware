from hw import *
from hw_input import key_down
from hw_input import Key

class Camera(ScriptableObject):
	def on_start(self):
		pass
	def on_update(self, ts):
		if key_down(Key.LEFT):
			self.rotation.z -= 1.2 * ts.get_seconds
		if key_down(Key.RIGHT):
			self.rotation.z += 1.2 * ts.get_seconds