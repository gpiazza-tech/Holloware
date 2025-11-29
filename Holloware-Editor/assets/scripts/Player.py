from hw import *
from hw_input import key_down
from hw_input import Key

class Player(ScriptableObject):
	run_speed = 6
	rotation_speed = 10

	def on_start(self):
		pass
	def on_update(self, ts):
		if key_down(Key.W):
			self.position.y += self.run_speed * ts.get_seconds
		if key_down(Key.S):
			self.position.y -= self.run_speed * ts.get_seconds
		if key_down(Key.A):
			self.rotation.z += self.rotation_speed * ts.get_seconds
			self.position.x -= self.run_speed * ts.get_seconds
		if key_down(Key.D):
			self.rotation.z -= self.rotation_speed * ts.get_seconds
			self.position.x += self.run_speed * ts.get_seconds