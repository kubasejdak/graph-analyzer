from django.db import models

# ShellcodeSample table
class Sample(models.Model):
	name = models.CharField(max_length = 100)
	extracted_from = models.CharField(max_length = 100)
	graph_name = models.CharField(max_length = 100)
	file_type = models.CharField(max_length = 20)
	file_size = models.CharField(max_length = 10)
	shellcode_offset = models.CharField(max_length = 10)
	
	def __unicode__(self):
		return self.name

# SyscallAnalyzer table
class API(models.Model):
	sample = models.ForeignKey(Sample)
	dll = models.CharField(max_length = 20)
	syscall = models.CharField(max_length = 20)

	def __unicode__(self):
		return self.syscall

# GraphHash table
class Hash(models.Model):
	sample = models.ForeignKey(Sample)
	hash = models.CharField(max_length = 10)

	def __unicode__(self):
		return self.hash

# LoopDetector table
class Loop(models.Model):
	sample = models.ForeignKey(Sample)
	hash = models.CharField(max_length = 10)
	iterations = models.CharField(max_length = 10)
	size = models.CharField(max_length = 10)
	start = models.CharField(max_length = 10)
	vertexes = models.CharField(max_length = 2000)
	
	def __unicode__(self):
		return self.hash