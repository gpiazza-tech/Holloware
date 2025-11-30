from hw import Vec3
from hw import Math
from hw_input import key_down
from hw_input import Key

class Player(ScriptableObject):
	run_speed = 6
	rotation_speed = 10
	isGrounded = False
	velocity = Vec3()
	gravity = 0.4
	jump_force = 0.07

	def on_start(self):
		pass
	def on_update(self, ts):
		if key_down(Key.A):
			# self.rotation.z += self.rotation_speed * ts.get_seconds
			self.position.x -= self.run_speed * ts.get_seconds
		if key_down(Key.D):
			# self.rotation.z -= self.rotation_speed * ts.get_seconds
			self.position.x += self.run_speed * ts.get_seconds
		self.position.y = Math.clamp(self.position.y, -2.0, 100.0)
		self.isGrounded = self.position.y == -2.0

		if not self.isGrounded:
			self.velocity.y -= ts.get_seconds * self.gravity
		else:
			self.velocity.y = 0.0

		if self.isGrounded and key_down(Key.SPACE):
			self.velocity.y = self.jump_force

		self.position.y += self.velocity.y