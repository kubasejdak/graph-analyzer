from django.db import models

# Task table
class Task(models.Model):
	name = models.CharField(max_length = 200)
	type = models.CharField(max_length = 20)
	start_time = models.CharField(max_length = 10)
	end_time = models.CharField(max_length = 10)
	work_time = models.CharField(max_length = 10)
	errors = models.IntegerField()
	finished = models.BooleanField()
	trait_name = models.CharField(max_length = 20)
	trait_value = models.CharField(max_length = 30)
	progress = models.IntegerField()
	
	def __unicode__(self):
		return self.name