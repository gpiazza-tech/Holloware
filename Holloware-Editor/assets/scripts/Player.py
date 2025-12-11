from hw import Vec3
from hw import Math
from hw_input import key_down
from hw_input import Key

class Player(ScriptableObject):
	def __init__(self):
		self.run_speed = 9.0
		self.rotation_speed = 0.0
		self.gravity = 0.8
		self.jump_force = 0.31

		self._is_grounded = False
		self._velocity = Vec3()
	def on_start(self):
		pass
	def on_update(self, ts):
		if key_down(Key.A):
			self.rotation.z += self.rotation_speed * ts.get_seconds
			self.position.x -= self.run_speed * ts.get_seconds
		if key_down(Key.D):
			self.rotation.z -= self.rotation_speed * ts.get_seconds
			self.position.x += self.run_speed * ts.get_seconds
		self.position.y = Math.clamp(self.position.y, -2.0, 100.0)
		self._is_grounded = self.position.y == -2.0

		if not self._is_grounded:
			self._velocity.y -= ts.get_seconds * self.gravity
		else:
			self._velocity.y = 0.0

		if self._is_grounded and key_down(Key.SPACE):
			self._velocity.y = self.jump_force

		self.position.y += self._velocity.y