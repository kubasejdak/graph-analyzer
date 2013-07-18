from django.db import models
from tasks.models import Task
from analyze.models import Sample

# Group table
class Group(models.Model):
    name = models.CharField(max_length = 200)
    leader = models.ForeignKey(Sample)
    members_num = models.IntegerField()
    comment = models.CharField(max_length = 3000)
    task = models.ForeignKey(Task)
    
    def __unicode__(self):
        return self.name
    
# GroupAssignment table
class GroupAssignment(models.Model):
    group = models.ForeignKey(Group)
    member = models.ForeignKey(Sample)
    resemblence = models.IntegerField()
    
    def __unicode__(self):
        return self.group.name
