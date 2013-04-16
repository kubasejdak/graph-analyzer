from django.db import models

# Task table
class Task(models.Model):
	name = models.CharField(max_length = 200)
	type = models.CharField(max_length = 20)
	start_time = models.TimeField()
	end_time = models.TimeField()
	work_time = models.TimeField()
	errors = models.IntegerField()
	finished = models.BooleanField()
	progress = models.IntegerField()
	
	def __unicode__(self):
		return self.name